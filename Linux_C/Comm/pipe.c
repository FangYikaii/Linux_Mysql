#include <stdio.h>
#include <unistd.h>   //API FOR POSIX OPERATING SYSTEM
#include <stdlib.h>
#include <string.h>

int pipe_default[2];

int main()
{
    pid_t pid;      //structure for process,declare a process pid
    char buffer[30];

    memset(buffer,0,32);  //将buffer指向的内存的32位内容设置为ascii值

    if(pipe(pipe_default)<0)   //如果pipe初始化失败
    {
        printf("Faild to create pipe!\n");
        return 0;
    }

    if(0==(pid=fork()))   //子进程
    {
        close(pipe_default[1]);    //关闭写管道
        sleep(5);        //给出时间让主进程有时间写管道

        if(read(pipe_default[0],buffer,32)>0)   //读管道
        {
            printf("Recevie data from server,%s!\n",buffer);
        }
        close(pipe_default[0]);    //关闭读管道
    }
    //父进程
    else
    {
        close(pipe_default[0]);  //关闭读管道
        if(-1!=write(pipe_default[1],"It's my test",strlen("It's my test")))  //写管道，写入str，长度为strlen（str）
        {
            printf("Send data,-- It's my test\n");
            
        }
        close(pipe_default[1]);    //关闭写管道
        waitpid(pid,NULL,0);       //等待子进程执行
    }
    return 1;
}

