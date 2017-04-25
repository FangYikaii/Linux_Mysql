#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int i=0;
    pid_t fpid;

    //ppid：当前进程的父进程pid
    //pid：当前进程的pid
    //fpid：fork返回给当前进程的值
    printf("i son/pa ppid pid fpid\n");
    
    for(i=0;i<2;i++)
    {
        fpid=fork();
        if(fpid==0)
            printf("%d child %4d %4d %4d \n",i,getppid(),getpid(),fpid);
        else
            printf("%d parent %4d %4d %4d \n",i,getppid(),getpid(),fpid);
    }

    return 0;
}
