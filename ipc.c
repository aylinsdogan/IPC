#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

void signalHandler(int signal);

struct sharedMemory
{
	pid_t processes[5];
	char currentProcessIdentifier[50];
	char message[1024];
	int count;
};

struct sharedMemory *ptr;

int processNo = -1;
int sharedMemoryId = -1;

int main(int argc, char **argv){

	if (argc == 1 || argc > 2){
		printf("Invalid arguement(s)\n");
		exit(-1);
	}

	char *processIdentifier = argv[1];
	if (strlen(processIdentifier) > 50){
		printf("Process identifier must be lower than 50 characters.\n");
		exit(-1);
	}

	sharedMemoryId = shmget(1234, sizeof(struct sharedMemory), 0644 | IPC_CREAT);

	if (sharedMemoryId == -1)
	{
		printf("Error creating shared memory ID\n");
		exit(-1);
	}

	ptr = (struct sharedMemory *)shmat(sharedMemoryId, NULL, 0);

	if (ptr == (void *)-1)
	{
		printf("Error attaching shared memory pointer\n");
		exit(-1);
	}
	
	if(ptr->count==0){
		for(int i=0; i<5; i++){
			ptr->processes[i] = -1;
		}
	}
	
	if (ptr->count >= 5){
		printf("Maximum 5 processes are allowed. \n");
		exit(1);
	}

	printf("--%s--\n", processIdentifier);
	signal(SIGINT, signalHandler);
	signal(SIGALRM, signalHandler);
    
	int emptyIndex=0;
	for(int i=0; i<5; i++){
		if(ptr->processes[i] == -1){
			emptyIndex = i;
			break;
		}
	}

	processNo = emptyIndex;
	ptr-> count++;
	pid_t pid = getpid();
	ptr-> processes[processNo] = pid;
	

	char message[1024];
	while (1){
	
		fgets(message, 1024, stdin);
		if (ptr->count < 2){
			continue;
		}

		strcpy(ptr->currentProcessIdentifier, processIdentifier);
		strcpy(ptr->message, message);
		for (int i = 0; i < 5; i++){
			pid_t processId = ptr->processes[i];
			if (processId != -1 && processId != pid){
				kill(processId, SIGALRM);
			}
		}
	}
}

void signalHandler(int signal)
{
	if (signal == SIGINT){ 
		if(ptr->count > 0){
			ptr->count--;
		}
		ptr->processes[processNo] = -1;
		shmdt(ptr);
		exit(1);
	}

	if (signal == SIGALRM)
	{
		printf("[%s]: %s\n", ptr->currentProcessIdentifier, ptr->message);
	}
}
