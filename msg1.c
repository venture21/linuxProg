#include<stdlib.h>		// EXIT_FAILURE, EXIT_SUCCESS
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct my_msg_st
{
	//메세지큐의 구조체에서 첫번째 데이터 타입 long int는 반드시 필요
	long int my_msg_type;	
	char some_text[BUFSIZ];
};

int main()
{
	int msgid;
	int running = 1;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;

	// step 1. msgget()
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	// 정상적으로 메세지큐가 생성되지 않은 경우
	if(msgid==-1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	// 메세지큐에서 데이터를 읽어오는 프로세스
	while(running)
	{
		// step 2. msgrcv()
		// 만약에 메세지큐로부터 데이터를 수신하는데 실패했다면,
		if(msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_receive, 0) ==-1)
		{
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		// 정상적으로 메세지큐로부터 데이터를 수신했다면, 문자열 데이터를 출력
		printf("You wrote : %s", some_data.some_text);

		// 만약 수신된 문자열이 "end"일 경우 
		if(strncmp(some_data.some_text, "end", 3)==0)
		{
			running = 0;	//while문을 빠져나가기 위한 조건 변경
		}
	}

	// step 3. msgctl로 메세지큐 삭제
	// 메세지큐 삭제에 실패할 경우,
	if(msgctl(msgid, IPC_RMID, 0)==-1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

