#include<stdlib.h>		// EXIT_FAILURE, EXIT_SUCCESS
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include "common.h"


int main()
{
	int msgid;
	int running = 1;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;
	char buffer[BUFSIZ];

	// step 1. msgget()
	msgid = msgget(KEY_VALUE, 0666 | IPC_CREAT);

	// 정상적으로 메세지큐가 생성되지 않은 경우
	if(msgid==-1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	// 메세지큐에 데이터를 보내는 프로세스
	while(running)
	{
		// 키보드로 문자열을 입력받는다.
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = 1;

		// strcpy함수를 사용하여 buffer에서 구조체안에 some_text로 문자열 복사
		strcpy(some_data.some_text, buffer);

		// step 2. msgsnd()
		// 만약에 메세지큐로 데이터를 보내는데 실패했다면,
		// MAX_TEXT의 사이즈는 (구조체 - long int) 사이즈 이다.
		if(msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) ==-1)
		{
			fprintf(stderr, "msgsnd failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		// 키보드로부터 입력 받은 데이터가 "end"일 경우 종료
		if(strncmp(some_data.some_text, "end", 3)==0)
		{
			running = 0;	//while문을 빠져나가기 위한 조건 변경
		}
	}

	exit(EXIT_SUCCESS);
}

