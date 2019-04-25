#include "SharedMemory.h"

int shmid;

int SHM_create()
{
	shmid = shmget((key_t)SHARED_MEMORY_KEY, MEMORY_SIZE, 0666|IPC_CREAT);
	
	if(shmid == -1)
	{
		perror("shmget failed: ");
		return -1;
	}
	
	return shmid;
}

int SHM_delete()
{
	if(shmctl(shmid, IPC_RMID, 0) == -1)
	{
		perror("shmctl failed: ");
		return -1;
	}
	
	return 1;
}