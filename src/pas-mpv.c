#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mpv/client.h>
#include <ctype.h>

size_t trimwhitespace(char *out, size_t len, const char *str)
{
  if(len == 0)
    return 0;

  const char *end;
  size_t out_size;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len-1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;
  return out_size;

}

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

    char tempargv1[4096];
    trimwhitespace(tempargv1, strlen(argv[1])+1, argv[1]);
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
    //check_error(mpv_set_option_string(ctx, "screen", "0"));
    check_error(mpv_set_option_string(ctx, "geometry", "1600x900"));
    check_error(mpv_set_property(ctx, "border", MPV_FORMAT_FLAG, &tru));
    int val = 0;
    check_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &val));
    // Done setting up options.
    check_error(mpv_initialize(ctx));

    // Play this file.
	int temptemptemptemp = 0;
/*	while(tempargv1[temptemptemptemp]!='\0'){
    		printf("%d %c||", tempargv1[temptemptemptemp], tempargv1[temptemptemptemp]);
		temptemptemptemp++;
}*/
printf("pipi\n");
    const char *cmd[] = {"loadfile", tempargv1, NULL};
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
    sprintf(tempfilenamedone, "/tmp/pas_disp_pipe_%d_done", getpid());
    tempfiledonepipe = fopen(tempfilenamedone, "w");
    fflush(tempfiledonepipe);
    mpv_terminate_destroy(ctx);
    return 0;
}
