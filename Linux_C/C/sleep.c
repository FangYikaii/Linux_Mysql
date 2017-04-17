#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int i;
    pid_t pid,wpid;
    pid=fork();

    if(0==pid)
    {
        printf("子进程号:%d。\n",getpid());
        sleep(5);
        exit(0);
    }
    else
    {
        printf("父进程等待....\n");
        wpid=wait(0);
        printf("结束的进程号：%d。\n",wpid);
    }
}
