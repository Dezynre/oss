#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <getopt.h>
 
struct Clock{
	int seconds;
	int nanosecond;
	int shmPID;
};

int main(int argc, char *argv[]){
	time_t seconds;
	char *shmADDRESS;
	int key=99999;
	int shmID;
	struct Clock clock;
	pid_t pid, mypid, myppid;
	int option;
	FILE *fp;
	fp=fopen("./logfile.txt","w");
	
	//Creation Of the Shared memory Segment
	shmID=shmget(key,sizeof(clock),IPC_CREAT | 0777);
	if (shmID==-1){
		printf("Shared Memory Segment Wasn't Created!\n");
	}else{
		printf("Shared Memory ID: %d\n", shmID);
		//Attachment of memory Segment to the master processes's memory page.
		shmADDRESS=shmat(shmID,NULL,0);
		if (!shmADDRESS){
			perror("shmat");
			exit(1);
			}
		printf("Address: %s\n", shmADDRESS);
		//Creating A child process that will read and write to the shared memory segment.
		pid=fork();

		if (pid==0){
			fprintf(fp,"Child Process Starts At: %ld\n", seconds);
			mypid=getpid();
			char *shmADDRESSS=shmat(shmID,NULL,0);
				if (!shmADDRESSS){
					perror("shmat");
					exit(1);
				}
			sleep(2);
			fprintf(fp,"Child Process Terminates At: %ld\n",seconds);
			exit(1);
		}
	}
	fclose(fp);
	return 0;
}