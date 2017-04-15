#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>

static int count=0;


static struct itimerval oldtv;  //结构，包含两个结构：当前时间（struct timeval it_interval）和下一个时间(struct timeval it_value)

void set_timer()
{
    struct itimerval itv;

    itv.it_interval.tv_sec=1;  //当前时间.秒
    itv.it_interval.tv_usec=0; //当前时间.毫秒
 
    itv.it_value.tv_sec=1;     //下一个时间.秒
    itv.it_value.tv_usec=0;    //下一个时间.毫秒

    setitimer(ITIMER_REAL,&itv,&oldtv);   //设置定时器，实现定时、延时的功能
}


void signal_handler(int m)
{
    count++;
    printf("%d\n",count);
}

int main()
{
    signal(SIGALRM,signal_handler);
    set_timer();
    while(count<10000);
    exit(0);
    return 1;
}
