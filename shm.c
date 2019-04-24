#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <stdio.h>

#include <string.h> 
#include <unistd.h> 

#define QUEUE_SIZE 10

// 생산자와 소비자간 공유할 데이터
struct data
{
    char name[80]; 
};

struct flock lock, unlock;

int lock_open(int fd, int index)
{
  lock.l_start  = index;
  lock.l_type = F_WRLCK;
  lock.l_len = 1;
  lock.l_whence = SEEK_SET;
  return fcntl(fd, F_SETLKW, &lock);
}

int lock_close(int fd, int index)
{
  unlock.l_start  = index;
  unlock.l_type = F_UNLCK;
  unlock.l_len = 1;
  unlock.l_whence = SEEK_SET;
  return fcntl(fd, F_SETLK, &unlock);
}

void lock_init()
{
  lock.l_start  = 0;
  lock.l_type = F_WRLCK;
  lock.l_len = 1;
  lock.l_whence = SEEK_SET;
}
void unlock_init()
{
  unlock.l_start  = 0;
  unlock.l_type = F_UNLCK;
  unlock.l_len = 1;
  unlock.l_whence = SEEK_SET;
}

int main()
{
	int shmid;
	int i = 0;
	int offset = 0;

	struct data *cal_num;
	void *shared_memory;
	struct data ldata;    
	int fd;

	lock_init();
	unlock_init();

	// 잠금 파일을 생성한다.
	if ((fd = open("shm_lock", O_CREAT|O_RDWR)) < 0)
	{
		perror("file open error ");
		exit(0);
	}
	// 파일을 공유메모리 큐의 크기만큼 만든다.
	write(fd, (void *)'\0', sizeof(char)*QUEUE_SIZE);

	// 공유메모리를 생성한다.
	// 공유메모리의 크기는 QUEUE_SIZE * 원소의 크기가 된다.  
	shmid = shmget((key_t)1234, sizeof(ldata)*QUEUE_SIZE, 0666|IPC_CREAT);
	if (shmid == -1)
	{
		perror("shmget failed : ");
		exit(0);
	}

	// 공유할 메모리의 크기를 할당하고 이를 공유 메모리영역에 붙인다.  
	shared_memory = (void *)malloc(sizeof(ldata)*QUEUE_SIZE);
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1)
	{
		perror("shmat failed : ");
		exit(0);
	}

	while(1)
	{
		// 공유할 데이터 
		sprintf(ldata.name, "write Data : %d\n",i);
		// 이건 디버깅용 출력물
		printf("%d %s",(i==0)? QUEUE_SIZE - 1:i-1, ldata.name);

		// 레코드를 잠근다.
		if(lock_open(fd, i)< 0)
		{
			perror("lock error");
		}
		// 레코드 잠금을 얻었다면
		// 이전 레코드의 잠금을 푼다.
		if(lock_close(fd, (i==0)? QUEUE_SIZE - 1: i-1) < 0)
		{
			perror("flock error");
		}
		// 공유메모리에 데이터를 쓴다.
		memcpy((void *)shared_memory+offset, (void *)&ldata, sizeof(ldata));
		sleep(1);
		offset += sizeof(ldata);
		i++;

		// 이건 순환 큐이다. 만약 큐의 크기를 모두 채웠다면 
		// offset과 인덱스 번호 i를 초기화 한다.
		if (i == QUEUE_SIZE) {i = 0; offset = 0;}
	}
}