#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// SIGINT에 대응하는 핸들러
void sigHandler(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);

     // signal handler 재등록
    (void) signal(SIGINT, SIG_DFL);
}

// main함수
int main()
{
    int i=3;
    
     // signal handler 등록
    signal(SIGINT, sigHandler);

    while(1) 
    {
        printf("Hello World!\n");
        sleep(1);
    }
}
