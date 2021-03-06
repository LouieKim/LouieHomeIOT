#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SHARED_MEMORY_KEY 1111
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
	shmid = shmget((key_t)SHARED_MEMORY_KEY, MEMORY_SIZE, 0666|IPC_CREAT);
	
	if(shmid == -1)
	{
		perror("shmget failed: ");
		exit(0);
	}
	
	//attach shared memory
	buffer = shmat(shmid, (void *)0, 0);
	
	if(buffer == (void *)-1)
	{
		perror("shmat failed:");
		exit(0);
	}
	string = buffer + 1;
	
	buffer[0] = READ_CLIENT_FLAG;
	
	while(1)
	{
		if(buffer[0] == READ_SERVER_FLAG)
		{
			puts(string);
			strcat(string, "by server");
			buffer[0] = PRINT_CLIENT_FLAG;
		}
		sleep(1);
	}
}
	
	