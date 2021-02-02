#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <bsd/unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <wordexp.h>
#include <sys/types.h>
#include <sys/wait.h>

int status;

struct entry{
    bool exists = 0;
    bool file;
    char filename[1024];
    bool message;
    char charmessage[4096];
};

char homedir[4096];
struct entry PA[64];

int pushentry(bool file, bool message, char filename[1024], char charmessage[4096]){
int i = 0;
//printf("pushing 1\n");
while(i<64){
if(!PA[i].exists){
    PA[i].file=file;
     memcpy(PA[i].filename,filename,sizeof(PA[i].filename));
    PA[i].message=message;
     memcpy(PA[i].charmessage,charmessage,sizeof(PA[i].charmessage));
    PA[i].exists = 1;
    return 1;
};
i++;
}
return 0;
};

int sendentry(){
     int oldPID = getpid();
//	printf("sending 1\n");
     char lockfilename[4096];
            sprintf(lockfilename, "/tmp/pas-disp-%d-done", oldPID);
    if(!access(lockfilename, F_OK)) {
    }
    else{
if(PA[0].exists){
//	printf("sending 2 \n");
    FILE* template_disp;
    char disp_command_prototype[4096];
    char temptemp[4096];
    sprintf(temptemp, "%s/PAS/paconfig/template-pas-disp", homedir);
    template_disp = fopen(temptemp, "r");
    if (template_disp!=NULL)
        {
            fscanf(template_disp, "`%[^`]", disp_command_prototype);
            fclose(template_disp);
        }
        else{
            printf("NO TEMPLATE_DISP!");
            abort();
        }
            PA[0].exists=0;
            char dispcommand[4096];
            sprintf(dispcommand, disp_command_prototype, PA[0].charmessage, PA[0].message, PA[0].file, PA[0].filename);
            int PID = fork();
            if(PID==0){
            fopen(lockfilename, "w");
            system(dispcommand);
            char removal[515];
            sprintf(removal,"rm %s", lockfilename);
            system(removal);
            exit(0);
            }
    }}
    int i = 0;
    if(PA[0].exists==0){
    while(i<63){
        PA[i].exists=PA[i+1].exists;
        PA[i].file=PA[i+1].file;
        PA[i].message=PA[i+1].message;
        strcpy(PA[i].charmessage, PA[i+1].charmessage);
        strcpy(PA[i].filename, PA[i+1].filename);
        i++;
    }
//    printf("moved to left \n");
    PA[64].exists=0;}
    }

int readFILEMAIL(int pid){ // Gets the data from the listened to FILEMAIL into the structure array.
                char tempfilename[512];
//		printf("reading 1\n");
                char tempfilenameready[512];
                sprintf(tempfilename, "/tmp/%d_FM", pid);
                sprintf(tempfilenameready, "/tmp/%d_r_FM", pid);
                FILE * FILEMAILpipe;
                FILE * FILEMAILpipeready;
                while((FILEMAILpipe = fopen(tempfilename, "r"))==NULL){usleep(10000);};
                while((FILEMAILpipeready=fopen(tempfilenameready, "r"))==NULL){usleep(10000);};
                bool file, tmessage;
                char filename[1024];
                char charmessage[4096];
                char temptemptemp[8192];
                fgets(temptemptemp, 4096, FILEMAILpipe);
                int failure = sscanf(temptemptemp, "I %d F %d i \"%[^\"]\" f \"%[^\"]\"", &file, &tmessage, charmessage, filename);
                int pushed = pushentry(file, tmessage, filename, charmessage);
                if(pushed){
//		printf("reading 2\n");
                char removal[515];
//		printf("reading n0\n");
                //fclose(FILEMAILpipe);
//		printf("reading n1\n");
                //fclose(FILEMAILpipeready);
//		printf("reading n2\n");
                sprintf(removal,"rm %s", tempfilename);
//              printf("reading n3\n");
		system(removal);
//		printf("reading n4\n");
		sprintf(removal,"rm %s", tempfilenameready);
                system(removal);
//		printf("reading 3\n");
                }
                return 0;
};

int listenToFILEMAIL(int pid){ // LISTENS to the specified PID FILEMAIL and sends the data if received to the structure table. Should be called every so often, at least once every 10 seconds.
                char tempfilename[512];
                sprintf(tempfilename, "/tmp/%d_FM", pid);
//		printf("waiting...\n");
if( access( tempfilename, F_OK ) == 0 ) {
    readFILEMAIL(pid);
}
return 0;
};

int main(int argc, char *argv[])
{

    snprintf(homedir, 4096, "%s", getenv("HOME"));
    long int lastread = 0;
    while(1){
      if(time(0)-lastread>0){
      listenToFILEMAIL(getpid());
      lastread=time(0);
    sendentry();
    waitpid(-1, &status, WNOHANG);}
    usleep(10000);
    }
}
