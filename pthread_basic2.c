#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


struct Data
{
	int x;
	int y;
};


void *t_function(void *data)
{
	struct Data data1;
	data1 = *((struct Data*)data);

	return (void*)(data1.x+data1.y);
}


int main(void)
{
	pthread_t thread;
	int ret;
	int status;
	struct Data data;
	data.x=11;
	data.y=3;

	ret = pthread_create(&thread, NULL, t_function, (void*)&data);

	if(ret<0)
	{	
		perror("Error : pthread_create():");
		exit(-1);
	}

	pthread_join(thread,(void**)&status);

	printf("return value:%d\n", status);

	return 0;
}
