#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h> //error output
#include <stdint.h> //uint8_t definitions

//wiring Pi
#include <wiringPi.h>
#include <wiringSerial.h>

#include "SharedMemory.h"

#define READ_CLIENT_FLAG 0
#define READ_SERVER_FLAG 1
#define PRINT_CLIENT_FLAG 2

char device[]= "/dev/ttyACM0";
// filedescriptor
int fd;
unsigned long baud = 9600;
//unsigned long time=0;

void signal_handler(int signo)
{
	printf("terminate Process\n");
	SHM_delete();
	exit(0);
}

void *t_function(void *data)
{
	printf("Test");
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

void *t_buffer()
{
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

void setup(){
 
  printf("%s \n", "Raspberry Startup!");
  fflush(stdout);
 
  //get filedescriptor
  if ((fd = serialOpen (device, baud)) < 0){
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    exit(1); //error
  }
 
  //setup GPIO in wiringPi mode
  if (wiringPiSetup () == -1){
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    exit(1); //error
  }
 
}
 
void loop()
{
	/*
  // Pong every 3 seconds
  if(millis()-time>=3000){
    serialPuts (fd, "Pong!\n");
    // you can also write data from 0-255
    // 65 is in ASCII 'A'
    serialPutchar (fd, 65);
    time=millis();
  }*/
 
  // read signal
  if(serialDataAvail (fd)){
    char newChar = serialGetchar (fd);
    printf("%c", newChar);
    fflush(stdout);
  }
 
}

void *t_usb_comm()
{
	setup();
  while(1) loop();
}

int main()
{
	signal(SIGINT, signal_handler);

	pthread_t p_thread[3];
	
	int thr_id;
	int status;
	
	char p1[] = "thread_1";
	char p2[] = "thread_2";
	char pM[] = "thread_m";
	
	sleep(1);
	
	thr_id = pthread_create(&p_thread[0], NULL, t_buffer, NULL);
	
	if(thr_id < 0)
	{
		perror("thread create error:1 ");
		exit(0);
	}
	
	thr_id = pthread_create(&p_thread[1], NULL, t_usb_comm, NULL);
	
	if(thr_id < 0)
	{
		perror("thread create error: ");
		exit(0);
	}
	
	thr_id = pthread_create(&p_thread[2], NULL, t_function, (void*)p2);
	
	if(thr_id < 0)
	{
		perror("thread create error: ");
		exit(0);
	}
	
	t_function((void *)pM);
	
	pthread_join(p_thread[0], (void **)&status);
	pthread_join(p_thread[1], (void **)&status);
	pthread_join(p_thread[2], (void **)&status);
	
	printf("when terminal\n");
	
	return 0;
}
