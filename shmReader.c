#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>

#define SHMCOUNT 100
#define SHMSIZE  sizeof(int)*SHMCOUNT
#define KEY_VALUE	(key_t)1234

int main(void)
{
	int shmid;
	void *shared_Mem=(void*)0;
	int *shmaddr;
	int i;

	// step 1. shmget()
	shmid = shmget(KEY_VALUE, SHMSIZE, 0666 | IPC_CREAT);

	// 공유메모리가 할당되지 않은 경우
	if(shmid==-1)
	{
		fprintf(stderr,"shmget failed with error\n");
		exit(EXIT_FAILURE);
	}

	// step 2. shmat()
	shared_Mem = shmat(shmid, (void*)0, 0);
	
	// shmat가 실패한 경우
	if(shared_Mem==(void*)-1)
	{
		fprintf(stderr,"shmat failed with error\n");
		exit(EXIT_FAILURE);
	}

	//shmat가 성공한 경우, 할당된 메모리 번지를 확인
	printf("Memory attatch at 0x%p\n", shared_Mem);
	shmaddr = (int*)shared_Mem;

	// step 3. memory access
	for(i=0;i<SHMCOUNT;i++)
	{
		printf("shmaddr:%p, data:%d\n", shmaddr+i, *(shmaddr+i));
	}

	// step 4. shmdt()
	// shmdt에 실패한 경우
	if(shmdt(shared_Mem)==-1)
	{
		fprintf(stderr,"shmdt failed with error\n");
		exit(EXIT_FAILURE);
	}

	// step 5. shmctl (IPC_RMID)
	if(shmctl(shmid, IPC_RMID, 0)==-1)
	{
		fprintf(stderr,"shmctl failed with error\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
