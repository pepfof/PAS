#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    // Enable default key bindings, so the user can actually interact with
    // the player (and e.g. close the window).
    check_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"));
    mpv_set_option_string(ctx, "input-vo-keyboard", "yes");
    mpv_set_option_string(ctx, "keep-open", "always");
    int val = 1;
    check_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &val));
    // Done setting up options.
    check_error(mpv_initialize(ctx));

    // Play this file.
    const char *cmd[] = {"loadfile", argv[1], NULL};
    check_error(mpv_command(ctx, cmd));
    
    // Let it play, and wait until the user quits.
    while (1) {
        mpv_event *event = mpv_wait_event(ctx, 10000);
        if (event->event_id == MPV_EVENT_SHUTDOWN)
            break;
        if (event->event_id == MPV_EVENT_PLAYBACK_RESTART){
            int val1 = 0;
            val1 = mpv_get_property(ctx, "duration", MPV_FORMAT_INT64, &val);
            printf("%d\n", val);
        }
        if (event->event_id == MPV_EVENT_PAUSE){
            printf("done\n");
        }
    }

    mpv_terminate_destroy(ctx);
    return 0;
}