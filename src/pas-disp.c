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


int file = 0;
int msg = 0;
int winy = 0;
int scroll = 1;
int done = 0;
int PIDmar = -2;

void term(int signum)
{
   done = 1;
                system("find . -wholename '/tmp/pas_*' -delete");
                kill(PIDmar, SIGKILL);
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

    char message[512], string[2048];
    char *argv0 = argv[0];
    int j;
    for (j=1; argv[j] && argv[j][0] == '-'; ++j) {  
            if (strcmp(argv[j], "-i") == 0) {
            if (sscanf (argv[++j], "%[^\"]", message) != 1) {
                fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory. -s for noscroll and -i for message");
                return(1);
            } }else
            if (strcmp(argv[j], "-s") == 0) {
            if (sscanf (argv[++j], "%[^\"]", string) != 1) {
                fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory. -s for noscroll and -i for message");
                return(1);
            } }else
            if (strcmp(argv[j], "-l") == 0) {
            if (sscanf (argv[++j], "%d", &scroll) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-f") == 0) {
            if (sscanf (argv[++j], "%d", &file) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-m") == 0) {
            if (sscanf (argv[++j], "%d", &msg) != 1) {
                return(1);
            }
            } else
            {
            fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory.");
            return(1);
        }
    }
   if (!strcmp(message, "")) strcpy(message, "DEFAULT_TEXT");
    argv += j;
    argc -= j;
    char mpv_command_prototype[512], marquee_command_prototype[512];
    FILE* template_mpv;
    FILE* template_marquee;
    char temptemp[4096];
    sprintf(temptemp, "%s/PAS/paconfig/template-pas-mpv", homedir);
    template_mpv = fopen(temptemp, "r");
    if (template_mpv!=NULL)
        {
            fscanf(template_mpv, "`%[^`]", mpv_command_prototype);
        }
    sprintf(temptemp, "%s/PAS/paconfig/template-pas-marquee", homedir);
    template_marquee = fopen(temptemp, "r");
    if (template_marquee!=NULL)
        {
            fscanf(template_marquee, "`%[^`]", marquee_command_prototype);
        }
    if(file){
            int PID = fork();
            if(PID==0){
            char temp[1024];
            sprintf(temp, mpv_command_prototype, message);
            wordexp_t p;
            char **w;
            wordexp(temp, &p, 0);
            w = p.we_wordv;
            PID = execvp(w[0],w);
            printf("%d", PID);
            }
            else{
                int a;
                char tempfiletemplatedone[]="/tmp/pas_mpv_pipe_%d_done";
                char tempfiletemplate[]="/tmp/pas_mpv_pipe_%d";
                char tempfiletemplateready[]="/tmp/pas_mpv_pipe_%d_ready";
                char tempfilenamedone[512];
                char tempfilename[512];
                char tempfilenameready[512];
               sprintf(tempfilenamedone, tempfiletemplatedone, PID);
               sprintf(tempfilename, tempfiletemplate, PID);
               sprintf(tempfilenameready, tempfiletemplateready, PID );
                FILE * mpvpipe;
                FILE * mpvpipeready;
                FILE * mpvpipedone;
                while((mpvpipe = fopen(tempfilename, "r"))==NULL){usleep(10000);};
                while((mpvpipeready=fopen(tempfilenameready, "r"))==NULL){usleep(10000);};
                fscanf(mpvpipe, "%d", &a);
                char removal[515];
                sprintf(removal,"rm %s", tempfilename);
                system(removal);
                sprintf(removal,"rm %s", tempfilenameready);
                system(removal);
                if(msg){
            PIDmar = fork();
            if(PIDmar==0){
            char temp[3072];
            char formatstring[3075];
            sprintf(formatstring, "\"%s\"", string);
            sprintf(temp, marquee_command_prototype, formatstring, a*1000, 1);
            wordexp_t p;
            char **w;
            wordexp(temp, &p, 0);
            w = p.we_wordv;
            execvp(w[0],w);
                }
                else{
                char tempfiletemplatemarqueedone[]="/tmp/pas_marquee_pipe_%d_done";
                char tempfilenamemarqueedone[512];
               sprintf(tempfilenamemarqueedone, tempfiletemplatemarqueedone, PIDmar);
                FILE * marpipedone;
                while((marpipedone=fopen(tempfilenamemarqueedone, "r"))==NULL){usleep(500000);};
                sprintf(removal,"rm %s", tempfilenamemarqueedone);
                system(removal);
                }}
                while((mpvpipedone=fopen(tempfilenamedone, "r"))==NULL){usleep(500000);};
                sprintf(removal,"rm %s", tempfilenamedone);
                system(removal);
                kill(PID, SIGKILL);
                if(msg) kill(PIDmar, SIGKILL);
            }
    }
    else {
        if(msg){
            PIDmar = fork();
                char removal[515];
            if(PIDmar==0){
            char temp[3072];
            char formatstring[3075];
            sprintf(formatstring, "\"%s\"", string);  
            sprintf(temp, marquee_command_prototype, formatstring, 1000, 1);
            wordexp_t p;
            char **w;
            wordexp(temp, &p, 0);
            w = p.we_wordv;
            execvp(w[0],w);
                }
                else{
                char tempfiletemplatemarqueedone[]="/tmp/pas_marquee_pipe_%d_done";
                char tempfilenamemarqueedone[512];
               sprintf(tempfilenamemarqueedone, tempfiletemplatemarqueedone, PIDmar);
                FILE * marpipedone;
                while((marpipedone=fopen(tempfilenamemarqueedone, "r"))==NULL){usleep(500000);};
                sprintf(removal,"rm %s", tempfilenamemarqueedone);
                system(removal);
                kill(PIDmar, SIGKILL);
                }}
    }
    char tempfiletemplatedone[]="/tmp/pas_disp_pipe_%d_done";
    char tempfilenamedone[512];
    sprintf(tempfilenamedone, tempfiletemplatedone, getpid());
    FILE * tempfiledonepipe;
    tempfiledonepipe = fopen(tempfilenamedone, "w");
    fflush(tempfiledonepipe);
}