#include <stdio.h>
#include <stdlib.h>

#include <sys/shm.h>
#include <unistd.h>

#define SHARED_MEMORY_KEY 1111
#define MEMORY_SIZE 1024

int SHM_create();
int SHM_delete();