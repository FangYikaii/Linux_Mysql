#include <stdio.h>
#include <string.h>
#incldue <pthread.h>
#incldue <errno.h>
#include <semaphore.h>

#define BUFSIZE    4
#define NUMBER     10

int sum_of_number=0;

struct recycle_buffer
{
    int buffer[BUFSIZE];
    int head,tail;
}re_buf;



//读线程用于唤醒写线程的条件变量
pthread_cond_t writer_wakeup=PTHREAD_COND_INITIALIZER;
//写线程用于唤醒写线程的条件变量
pthread_cond_t reader_wakeup=PTHREAD_COND_INITIALIZER;

//对写
