#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SHARED_MEMORY_KEY 1234
#define MEMORY_SIZE 1024

#define READ_CLIENT_FLAG 0
#define READ_SERVER_FLAG 1
#define PRINT_CLIENT_FLAG 2

int main()
{
	int shmid;
	char *buffer;
	char *string;
	
	//make space that shared-memory
	shmid = shmget((key_t)SHARED_MEMORY_KEY, MEMORY_SIZE, 0666 | IPC_CREAT);
	if(shmid == -1)
	{
		perror("shmget failed: ");
		exit(0);
	}
	
	//attach shared memory
	buffer = shmat(shmid, NULL, 0);
	if(buffer == (void *)-1)
	{
		perror("shmat failed: ");
		exit(0);
	}
	
	buffer[0] = READ_CLIENT_FLAG;
	string = buffer + 1;
	
	while(1)
	{
		if(buffer[0] == READ_CLIENT_FLAG)
		{
			printf("message : ");
			scanf("%s", string);
			
			buffer[0] = READ_SERVER_FLAG;
		}
		else if(buffer[0] == PRINT_CLIENT_FLAG)
		{
			puts(string);
			buffer[0] = READ_CLIENT_FLAG;
		}
		sleep(1);
	}
}			