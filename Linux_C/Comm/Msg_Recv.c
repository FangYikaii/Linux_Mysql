#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


#define MSGKEY 1024

struct msgstruct
{
    long msgtype;
    char msgtext[2048];
};



/*
 * 子进程，监听消息队列
 */
void childproc()
{
    struct msgstruct msg;   //接收消息队列的结构体
    int msg_id,ret_value;   //msg_id消息队列关键字,ret_value接收消息关键字
    char str[512];          //消息数据

    while(1)
    {
        //检查消息队列是否存在
        msg_id=msgget(MSGKEY,IPC_EXCL);

        if(msg_id<0)
        {
            printf("msg is not exist!errno=%d [%s]\n",errno,strerror(errno));
            sleep(2);
            continue;
        }

        //接收消息队列
        ret_value=msgrcv(msg_id,&msg,sizeof(struct msgstruct),0,0);
        printf("text=[%s] pid=[%d]\n",msg.msgtext,getpid());
    }
    return;
}

void main()
{
    int i,cpid;

    //创建子进程
    for(i=0;i<5;i++)
    {
        cpid=fork();

        if(cpid<0)       //创建失败
        {
            printf("fork failed!\n");
        }
        else if(cpid==0)   //创建成功：子进程
        {
            childproc();
        }

    }
}
