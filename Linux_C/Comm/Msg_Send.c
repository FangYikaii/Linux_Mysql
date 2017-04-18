#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MSGKEY 1024

struct msgstruct
{
    long msgtype;
   char msgtext[2048];
};


int main()
{
    struct msgstruct msg;       //发送消息队列的结构体
    int msg_type;               //变量：消息类型
    char str[256];              //变量：消息数据
    int ret_value;              //发送消息关键字
    int msg_id;                 //消息队列关键字
    
    msg_id=msgget(MSGKEY,IPC_EXCL);   //检查消息队列是否存在
    if(msg_id<0)                      //如果消息队列不存在
    {
        msg_id=msgget(MSGKEY,IPC_CREAT|0666);   //创建消息队列
        if(msg_id<0)
        {
            printf("Failed to create msg|errno=%d [%s]\n",errno,strerror(errno));
            exit(-1);
        }
    }

    while(1)
    {
        printf("Input message type(end:0)\n");
        scanf("%d",&msg_type);
        if(msg_type==0)
        {
            break;
        }
        
        printf("Input message to be send\n");
        scanf("%s",str);
        
        msg.msgtype=msg_type;
        strcpy(msg.msgtext,str);

        //发送消息队列
        ret_value=msgsnd(msg_id,&msg,sizeof(struct msgstruct),IPC_NOWAIT);  //IPC_NOWAIT:读写消息队列的要求无法满足的时候 ，不阻塞

        if(ret_value<0)       //发送失败的错误处理
        {
            printf("msgsnd() write msg failed,errno=%d[%s]\n",errno,strerror(errno));
            exit(-1);
        }
    }
    
    msgctl(msg_id,IPC_RMID,0);

}
