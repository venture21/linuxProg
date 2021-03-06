#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>


static int sigHandler(int sig)
{
    static int count=0;

    printf("sigHandler : %d\n", count);
    count++;

}

int main(int argc, char *argv[])
{
    int i, status;
    int sig, result;
    pid_t pid;


    pid= fork();

    // 정상적으로 자식 프로세스가 생성되지 않은 경우
    if(pid==-1)
    {
        printf("Error:fork()");
        exit(-1);
    }
    // 부모 프로세스가 실행할 구문
    else if(pid>0)
    {
        for(i=0;i<10;i++)
        {
            result = kill(pid, SIGINT);
       
            // signal이 정상적으로 전송되지 않은 경우
            if(result==-1)
            {
                printf("Error: kill()\n");
                exit(-2);
            }
            sleep(1);
        }
        wait(&status);
        exit(0);
    }
    //자식 프로세스인 경우
    else
    {
        signal(SIGINT, sigHandler);
        for(i=0;i<10;i++)
        {
            pause();
            printf("Hello\n");
        }
        exit(0);
    }
}
