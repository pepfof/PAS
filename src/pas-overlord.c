#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <wordexp.h>

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
     char lockfilename[4096];
            sprintf(lockfilename, "/tmp/pas-disp-%d-done", oldPID);
            printf("%s ", lockfilename);
            printf("%d \n", access(lockfilename, F_OK) ==  0);
    if(!access(lockfilename, F_OK)) {
                char removal[4100];
                sprintf(removal,"rm %s", lockfilename);
                system(removal);
    }
    else{
    if(PA[0].exists){
            printf("no entry available.\n");
    FILE* template_disp;
    char disp_command_prototype[4096];
    char temptemp[4096];
    sprintf(temptemp, "%s/PAS/paconfig/template-pas-disp", homedir);
    template_disp = fopen(temptemp, "r");
    if (template_disp!=NULL)
        {
            fscanf(template_disp, "`%[^`]", disp_command_prototype);
        }
            PA[0].exists=0;
            char dispcommand[4096];
            sprintf(dispcommand, disp_command_prototype, PA[0].charmessage, PA[0].message, PA[0].file, PA[0].filename);
            int PID = fork();
            if(PID==0){
            printf("%s\n", dispcommand);
            system(dispcommand);
            fopen(lockfilename, "w");
            printf("we're done here!\n");
            _exit(0);
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
    PA[64].exists=0;}
    }

int readFILEMAIL(int pid){ // Gets the data from the listened to FILEMAIL into the structure array.
                char tempfilename[512];
                char tempfilenameready[512];
                sprintf(tempfilename, "/tmp/%d_FM", pid);
                printf("file is %s\n", tempfilename);
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
                int failure = sscanf(temptemptemp, "I %c F %d i \"%[^\"]\" f \"%[^\"]\"", &tmessage, &file, charmessage, filename);
                printf("%s AAAAAAAAAAAAAAAAAAAAAAAAAAAA I %i F %i i %s f %d\n",temptemptemp, tmessage, file, charmessage, failure);
                int pushed = pushentry(file, tmessage, filename, charmessage);
                if(pushed){
                char removal[515];
                sprintf(removal,"rm %s", tempfilename);
                system(removal);
                sprintf(removal,"rm %s", tempfilenameready);
                system(removal);
                }
                return 0;
};

int listenToFILEMAIL(int pid){ // LISTENS to the specified PID FILEMAIL and sends the data if received to the structure table. Should be called every so often, at least once every 10 seconds.
                char tempfilename[512];
                sprintf(tempfilename, "/tmp/%d_FM", pid);
    printf("file is: %s\n", tempfilename);
if( access( tempfilename, F_OK ) == 0 ) {
    printf("file found: %s\n", tempfilename);
    readFILEMAIL(pid);
}
return 0;
};

int submitPA(){
    if(PA[0].exists){
        /* send! */}
    else{
        int i = 64;
        int c = 0;
        while(i--){
            if(PA[0].exists){c = 1;}
        }
        if(c) submitPA();
    }
}

int main(int argc, char *argv[])
{

    snprintf(homedir, 4096, "%s", getenv("HOME"));
    long int lastread = 0;
    while(1){
      if(time(0)-lastread>0){
      listenToFILEMAIL(getpid());
      lastread=time(0);
    sendentry();}
    usleep(10000);
    }
}