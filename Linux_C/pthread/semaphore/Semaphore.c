#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>


#define BUFSIZE 4
#define NUMBER  100

//生产者和消费者共享一个循环队列，对循环队列进行同步控制，实现生产者和消费者的协同工作


int sum_of_number=0;

//分别表示可写和可读资源数
sem_t write_res_number;   //信号量指针
sem_t read_res_number;

//循环队列
struct recycle_buffer
{
    int buffer[BUFSIZE];  
    int head,tail;
}ref_buf;



//用于实现临界区的互斥锁
//如果一个线程已经对互斥锁上锁，其他线程则不能再对其上锁，这些线程将被阻塞等待
//直到上锁的线程将锁打开，其他线程才能对锁进行上锁，如果多个线程同时在等待一把锁，他们将竞争

pthread_mutex_t buffer_mutex=PTHREAD_MUTEX_INITIALIZER;



static void* producer(void* arg)    //函数代表线程函数指针，arg代表传递给线程函数的参数
{
    int i;
    //生产者将写资源信号量的值-1,表示可写单元减少1,而后进入临界区将数据复制到循环队列的尾指针处
    //离开后，将可读资源信号量加1,表示可读单元+1
    for(i=0;i<=NUMBER;i++)
    {
        //减少可写的资源数
        sem_wait(&write_res_number);
        //进入互斥区
        pthread_mutex_lock(&buffer_mutex);
        //将数据复制到缓冲区的尾部
        ref_buf.buffer[ref_buf.tail]=i;
        ref_buf.tail=(ref_buf.tail+1)%BUFSIZE;
        printf("producer %d write %d.\n",(int)pthread_self(),i);   //自身线程，写的数字
        //离开互斥区
        pthread_mutex_unlock(&buffer_mutex);
        //增加可读资源数
        sem_post(&read_res_number);
    }
    //线程终止，如果有线程在等待他们结束，则将把NULL作为等待其结束的返回值
    return NULL;
}

static void* consumer(void* arg)
{
    int i,num;
    //消费者将可读资源-1,表示可读单元减少1,而后进入临界区将数据从头指针处复制到自己的变量中
    //离开临界区后，将可写的资源信号量的值+1,表示可写单元+1
    for(i=0;i<=NUMBER;i++)
    {
        //减少可读资源数
        sem_wait(&read_res_number);
        //进入互斥区
        pthread_mutex_lock(&buffer_mutex);
        //从缓冲区头部获取出数据
        num=ref_buf.buffer[ref_buf.head];
        ref_buf.head=(ref_buf.head+1)%BUFSIZE;
        printf("consumer %d read %d.\n",(int)pthread_self(),num);
        //离开互斥区
        pthread_mutex_unlock(&buffer_mutex);
        sum_of_number+=num;
        //增加可写资源数
        sem_post(&write_res_number);
    }
    //线程终止，如果有线程在等它们结束，则把NULL作为等待其结束的返回值
    return NULL;
}

void main(int argc,char** argv)
{
    //用于保存线程的线程号
    pthread_t p_tid;
    pthread_t c_tid;
    int i;

    ref_buf.head=0;
    ref_buf.tail=0;
    for(i=0;i<BUFSIZE;i++)
    {
        ref_buf.buffer[i]=0;
    }

    //初始化可写资源数为循环队列的单元数
    sem_init(&write_res_number,0,BUFSIZE);
    //初始化可读资源为0
    sem_init(&read_res_number,0,0);

    //创建两个线程，线程函数分别是producer和consumer
    //这两个线程将使用系统缺省的线程设置，如线程的堆栈大小、线程调度策略和相应的优先级
    pthread_create(&p_tid,NULL,producer,NULL);
    pthread_create(&c_tid,NULL,consumer,NULL);
    //等待两个线程完成退出
    pthread_join(p_tid,NULL);
    pthread_join(c_tid,NULL);

    printf("The sum of number is %d\n",sum_of_number);
}


