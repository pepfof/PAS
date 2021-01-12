#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpv/client.h>
#include <string.h>
#include <signal.h>
#include <wordexp.h>
#include <cerrno>
#include <sys/stat.h>
#include <signal.h>


int tfile = 0;
int msg = 0;
int winy = 0;
int scroll = 1;
int done = 0;
int PIDmar = -2;

void term(int signum)
{
                _exit(0);
}


char homedir[4096];
int main(int argc, char *argv[])
{
    snprintf(homedir, 4096, "%s", getenv("HOME"));
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = term;
    sigaction(SIGINT, &action, NULL);

    char message[4096], string[4096];
    char *argv0 = argv[0];
    int j;
    for (j=1; argv[j] && argv[j][0] == '-'; ++j) {  
            if (strcmp(argv[j], "-i") == 0) {
            if (sscanf (argv[++j], "%[^\"]", message) != 1) {
                fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory. -s for noscroll and -i for message");
                return(1);
            } }else
            if (strcmp(argv[j], "-f") == 0) {
            if (sscanf (argv[++j], "%[^\"]", string) != 1) {
                fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory. -s for noscroll and -i for message");
                return(1);
            } }else
            if (strcmp(argv[j], "-F") == 0) {
            if (sscanf (argv[++j], "%d", &tfile) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-I") == 0) {
            if (sscanf (argv[++j], "%d", &msg) != 1) {
                return(1);
            }
            } else
            {
            fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory.");
            return(1);
        }
    }
FILE *pipe;
FILE *fp;

char input[128];
fp = popen("pidof pas-overlord", "r");
int pidofoverlord;
while (fgets(input, 128, fp) != NULL){
    sscanf(input, "%d", &pidofoverlord);
}
    char file[4096];
    sprintf(file, "/tmp/%d_FM", pidofoverlord);
    char rfile[4096];
    sprintf(rfile, "/tmp/%d_r_FM", pidofoverlord);
    pipe = fopen(file, "w");
    char tempfilenamedone[512];
    fprintf(pipe, "I %d F %d i \"%s \" f \"%s \"", tfile, msg, message, string);
    fflush(pipe);
    fclose(pipe);
    pipe = fopen(rfile, "w");
    fflush(pipe);
}