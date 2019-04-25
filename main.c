#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "SharedMemory.h"

#define READ_CLIENT_FLAG 0
#define READ_SERVER_FLAG 1
#define PRINT_CLIENT_FLAG 2

void signal_handler(int signo)
{
	printf("terminate Process\n");
	SHM_delete();
	exit(0);
}

void *t_function(void *data)
{
	pid_t pid;
	pthread_t tid;
	
	pid = getpid();
	tid = pthread_self();
	
	char* thread_name = (char*)data;
	int i = 0;
	
	while(i<3)
	{
		printf("[%s] pid:%u, tid:%x --- %d\n",
			thread_name, (unsigned int)pid, (unsigned int)tid, i);
		i++;
		sleep(1);
	}
}

void *t_buffer(char *buffer, char *string)
{
	while(1)
	{
		if(buffer[0] == READ_SERVER_FLAG)
		{
			puts(string);
			strcat(string, "by server");
			buffer[0] = PRINT_CLIENT_FLAG;
		}
		printf("wait data\n");
		sleep(1);
	}
}
	
int main()
{
	signal(SIGINT, signal_handler);
	
	int shmid;
	shmid = SHM_create();
	
	char *buffer;
	char *string;
	
	buffer = shmat(shmid, (void *)0, 0);
	if(buffer == (void *)-1)
	{
		perror("shmat failed: ");
	}
	
	buffer[0] = READ_CLIENT_FLAG;
	string = buffer + 1;	
	
	pthread_t p_thread[2];
	
	int thr_id;
	int status;
	
	char p1[] = "thread_1";
	char p2[] = "thread_2";
	char pM[] = "thread_m";
	
	sleep(1);
	
	thr_id = pthread_create(&p_thread[0], NULL, t_buffer, (void*)p1);
	
	if(thr_id < 0)
	{
		perror("thread create error: ");
		exit(0);
	}
	
	thr_id = pthread_create(&p_thread[1], NULL, t_function, (void*)p2);
	
	if(thr_id < 0)
	{
		perror("thread create error: ");
		exit(0);
	}
	
	t_function((void *)pM);
	
	pthread_join(p_thread[0], (void **)&status);
	pthread_join(p_thread[1], (void **)&status);
	
	printf("when terminal\n");
	
	return 0;
}