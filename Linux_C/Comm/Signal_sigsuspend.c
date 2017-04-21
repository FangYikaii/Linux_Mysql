#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int received_signal=0;

void catch_ctrl_c(int signo)
{
    printf("catch signal %d.\n",signo);
    printf("set receive signal flag.\n");
    received_signal=1;
}

void main(int argc,char** argv)
{
    sigset_t newmask;         //信号集合结构
    struct sigaction action;  //sigaction结构：sa_handler是函数指针，指向信号处理器的入口（忽略SIG_IGN,缺省SIG_DFL）;sa_mask:进程屏蔽信号的集合；sa_flags：信号处理器的一些标志

    /*SET UP THE CTRL+C SIGNAL HANDLER*/
    action.sa_handler=catch_ctrl_c;        //action的函数指针
    sigemptyset(&action.sa_mask);          //屏蔽信号集合设置为空
    action.sa_flags=0;                     //标志设置为0
    if(sigaction(SIGINT,&action,NULL)==-1)  //初始化action
    {
        perror("can not install the handler for SIGINT");
    }
    sigprocmask(SIG_SETMASK,NULL,&newmask);    //SIG_SETMASK将信号屏蔽码设置为给定信号的集合
    sigaddset(&newmask,SIGINT);

    //SIG_BLOCK阻塞进程期望的信号
    sigprocmask(SIG_BLOCK,&newmask,NULL);
    //为sigsuspend返回做准备
    sigdelset(&newmask,SIGINT);
    printf("waiting for signal SIGINT.\n");
    //检测变量
    while(received_signal==0)
    {
        sigsuspend(&newmask);    //在进程检测变量前将进程希望的信号阻塞。然后进程检测变量，如果事件没有发生，调用sigsuspend,将用提供的信号屏蔽码取代进程原来的屏蔽码，然后和pause一样直到收到信号并执行完信号处理后，才返回
    }
}
