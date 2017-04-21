#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define NUM_SEMAPHORE 1         //实现临界区，自需要一个信号量
#define SEM_KEY 0x11223333      //信号量的key值，用户自己定义，但要确保唯一
#define MUTEX 0                 //信号量在信号量组中的编号

int sem_id;                     //存放信号量组的ID

void P(int sem_num)
{
    struct sembuf sem[1];
    sem[0].sem_num=sem_num;   //对信号量的第sem_num进行操作
    sem[0].sem_op=-1;         //对信号量的第sem_num执行的操作
    sem[0].sem_flg=0;         //操作选项
    //定义一个操作，对信号量组的sem_num个信号量上试图减一
    //如果此时资源不足，进程将在semop系统调用中阻塞
    if(semop(sem_id,sem,1)==-1)
    {
        perror("P operation");
        exit(1);
    }
}

void V(int sem_num)
{
    struct sembuf sem[1];
    sem[0].sem_num=sem_num;
    sem[0].sem_op=1;
    sem[0].sem_flg=0;
    //定义一个V操作，释放一个资源
    if(semop(sem_id,sem,1)==-1)
    {
        perror("V operation");
        exit(1);
    }
}

void catch_ctrl_c(int signo)
{
    //在收到ctrl+c后，进程将删除信号量组，然后退出
    if(semctl(sem_id,0,IPC_RMID,0)==-1)
    {
        perror("Remove Semaphore");
    }
    else
    {
        printf("Remove Semaphore ok.\n");
    }
    exit(0);
}

void main(int argc,char** argv)
{
    int fd,ticket_number,ticket_want;     //标识符
    sigset_t old_set,new_set;             //信号集
    struct sigaction action;              //函数处理结构体
    struct sembuf ops[1];                 //信号量组

    fd=open("ticket.txt",O_RDWR);
    if(fd<0)
    {
        perror("can not open ticket.txt");
        exit(1);
    }
    
    //安装信号处理器
    action.sa_handler=catch_ctrl_c;
    //屏蔽信号集合设置为空
    sigemptyset(&action.sa_mask);
    //标志设置为0
    action.sa_flags=0
    //初始化action
    if(sigaction(SIGINT,&action,NULL)==-1)      //SIGINT:ctrl+c键盘发出的中断信号
    {
        perror("Can not install the handler for SIGINT");
    }

    //判断信号量组是否存在
    sem_id=semget(SEM_KEY,0,0);  //SEM_KEY:信号量的key值
    if(sem_id!=-1)   //如果信号量组不存在，创建信号量组
    {
        sem_id=semget(SEM_KEY,NUM_SEMAPHORE,IPC_CREAT|OPC_EXCL|0666);
        if(sem_id==-1)
        {
            perror("Create Semaphore Error");
            exit(1);
        }
        else  //如果初始化成功，将资源数设置为1
        {
            printf("Create Semaphores:OK\n");
            ops[0].sem_num=MUTEX;
            ops[0].sem_op=1;
            ops[0].sem_flg=0;
            if(semop(sem_id,ops,1)==-1)
            {
                perror("semop");
                exit(1);
            }
            else
            {
                printf("Initialize Semaphore Succeed!\n");
            }
        }        
    }

    //new_set信号集为空
    sigemptyset(&new_set);
    //添加SIGINT
    sigaddset(&new_set，SIGINT);

    while(1)
    {
        printf("How many tickets do you want?");
        scanf("%d",&ticket_want);

    }
    

}
