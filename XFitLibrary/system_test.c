/* Test of running a system program */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

void main(int argc, char * argv []) {
pid_t newpid;
int status,execstatus,i;
if(argc < 2) {
    printf("Not enough arguments\n");
    exit(-1);
    }

for(i=0; i< argc; i++) {
    printf("Argument %d: %s\n",i,argv[i]);
    }

if((newpid = fork())!= -1)  {
    if(newpid == 0)  {    /* child process */
        printf("Will execute %s with arguments %s\n", argv[1],argv[2]);
        execstatus = execvp(argv[1], argv+1);  /* execute...*/
        if(execstatus == -1) {
           perror("Failed to execute: ");
           }
        }
    else {                /* parent process */
        waitpid(newpid,&status,NULL);
        printf("%s exited with status %d\n",*(argv+1),status);
        }
    }
}
