#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mpv/client.h>

static inline void check_error(int status)
{
    if (status < 0) {
        printf("mpv API error: %s\n", mpv_error_string(status));
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("pass a single media file as argument\n");
        return 1;
    }

    mpv_handle *ctx = mpv_create();
    if (!ctx) {
        printf("failed creating context\n");
        return 1;
    }
    int tru = 0;
    int screen = 0;
    // Enable default key bindings, so the user can actually interact with
    // the player (and e.g. close the window).
    check_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"));
    mpv_set_option_string(ctx, "input-vo-keyboard", "yes");
    mpv_set_option_string(ctx, "keep-open", "always");
    check_error(mpv_set_option_string(ctx, "screen", "1"));
    check_error(mpv_set_option_string(ctx, "geometry", "100%x100%"));
    check_error(mpv_set_property(ctx, "border", MPV_FORMAT_FLAG, &tru));
    int val = 0;
    check_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &val));
    // Done setting up options.
    check_error(mpv_initialize(ctx));

    // Play this file.
    const char *cmd[] = {"loadfile", argv[1], NULL};
    check_error(mpv_command(ctx, cmd));
    
    char tempfiletemplatedone[]="/tmp/pas_mpv_pipe_%d_done";
    char tempfiletemplate[]="/tmp/pas_mpv_pipe_%d";
    char tempfiletemplateready[]="/tmp/pas_mpv_pipe_%d_ready";
    char tempfilenamedone[512];
    char tempfilename[512];
    char tempfilenameready[512];
    sprintf(tempfilenamedone, tempfiletemplatedone, getpid());
    sprintf(tempfilename, tempfiletemplate, getpid());
    sprintf(tempfilenameready, tempfiletemplateready, getpid());
    FILE * tempfilepipe;
    FILE * tempfilereadypipe;
    FILE * tempfiledonepipe;
    while (1) {
        mpv_event *event = mpv_wait_event(ctx, 10000);
        if (event->event_id == MPV_EVENT_SHUTDOWN)
            break;
        if (event->event_id == MPV_EVENT_PLAYBACK_RESTART){
            int val1 = 0;
            val1 = mpv_get_property(ctx, "duration", MPV_FORMAT_INT64, &val);
            tempfilepipe = fopen(tempfilename, "w");
            fprintf(tempfilepipe, "%d\n", val);
            fflush(tempfilepipe);
            tempfilereadypipe = fopen(tempfilenameready, "w");
            fflush(tempfilereadypipe);
        }
        if (event->event_id == MPV_EVENT_PAUSE){
            tempfiledonepipe = fopen(tempfilenamedone, "w");
            fflush(tempfiledonepipe);
        }
    }

    mpv_terminate_destroy(ctx);
    return 0;
}