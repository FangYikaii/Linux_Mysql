#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

#define NUM_SEMAPHORE 2         //实现临界区，自需要一个信号量
#define SEM_KEY 0x11223332      //信号量的key值，用户自己定义，但要确保唯一
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

    printf("1test!\n");

    //安装信号处理器
    action.sa_handler=catch_ctrl_c;
    //屏蔽信号集合设置为空
    sigemptyset(&action.sa_mask);
    //标志设置为0
    action.sa_flags=0;
    //初始化action
    if(sigaction(SIGINT,&action,NULL)==-1)      //SIGINT:ctrl+c键盘发出的中断信号
    {
        perror("Can not install the handler for SIGINT");
    }

    //判断信号量组是否存在
    sem_id=semget(SEM_KEY,0,0);  //SEM_KEY:信号量的key值
    if(sem_id!=-1)   //如果信号量组不存在，创建信号量组
    {
        sem_id=semget(SEM_KEY,NUM_SEMAPHORE,IPC_CREAT|IPC_EXCL|0666);
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

    printf("test!\n");
    //new_set信号集为空
    sigemptyset(&new_set);
    //添加SIGINT
    sigaddset(&new_set,SIGINT);

    while(1)
    {
        printf("How many tickets do you want?");
        scanf("%d",&ticket_want);
        //由于在sigint的信号处理器中删除信号量组，为了避免修改票数的操作被信号打断，在进行操作前阻塞这个信号
        sigprocmask(SIG_BLOCK,&new_set,&old_set);
        //将资源减少1，由于资源被初始化为1，所以其他进程都只能在临界区外等待
        P(MUTEX);
        lseek(fd,SEEK_SET,0);
        //从文件中读取数据
        if(read(fd,(void*)&ticket_number,sizeof(int))<sizeof(int))
        {
            printf("read from ticket.txt error");
            exit(1);
        }

        printf("There are %d tickets left./n",ticket_number);
        if(ticket_number>ticket_want)
        {
            ticket_number=ticket_want;
            //故意睡眠增加处于临界区的时间
            sleep(2);
            lseek(fd,SEEK_SET,0);
            //将数据写回文件
            if(write(fd,&ticket_number,sizeof(int))<sizeof(int))
            {
                printf("write to ticket.txt error.\n");
                exit(1);
            }
            printf("There are %d tickets left now.\n",ticket_number);
            printf("booking ticket ok.\n");

        }
        else
        {
            printf("sorry,there are no so many tickets left.\n");
        }
        V(MUTEX);
        //清除对信号SIGINT的阻塞
        sigprocmask(SIG_SETMASK,&old_set,NULL);
    }
}
