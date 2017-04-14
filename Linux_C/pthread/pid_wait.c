#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t childpid;
    int status;

    childpid=fork();

    //如果fork失败
    if(childpid==-1)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    //子进程，fork返回0
    else if(childpid==0)
    {
        puts("In child process");
        sleep(3);
        printf("\tchild pid=%d\n",getpid());
        printf("\tchild ppid=%d\n",getppid());
        exit(EXIT_SUCCESS);
    }
    //父进程，fork返回子进程的id
    else
    {
//        waitpid(childpid,&status,0);  //线程等待，等到子进程执行完毕,status返回子进程的状态
        puts("In parent");
        printf("\tparent pid=%d\n",getpid());
        printf("\tparent ppid=%d\n",getppid());
        printf("\tchild process exited with status %d \n",status);  //查看子进程状态
    }
    exit(EXIT_SUCCESS);
}
