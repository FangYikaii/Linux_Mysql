#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>

struct msg_buf
{
    int mtype;
    char data[255];
};

int main()
{
    key_t key;
    int msgid;
    int ret;
    struct msg_buf msgbuf;
    key=ftok("./tmp/msg",100);

    printf("key = [%d] \n",key);


    //创建一个消息队列
    msgid=msgget(key,IPC_CREAT|0666);      //msgid 消息队列描述符
    if(msgid==-1)
    {
        printf("Create error\n");
        return -1;
    }


    //获取进程ID
    msgbuf.mtype=getpid();
    //获取发送内容
    strcpy(msgbuf.data,"This is my msg test..");
    printf("send msg data=[%s]\n",msgbuf.data);

    //发送消息
    ret=msgsnd(msgid,&msgbuf,sizeof(msgbuf.data),IPC_NOWAIT);
    if(ret==-1)
    {
        printf("Send message error!\n");
        return -1;
    }

    memset(&msgbuf,0,sizeof(msgbuf));

    //接收消息 
    ret=msgrcv(msgid,&msgbuf,sizeof(msgbuf.data),getpid(),IPC_NOWAIT);
    
    if(ret==-1)
    {
        printf("Recv message error\n");
     //   return -1;
    }

    printf("recv msg data=[%s]\n",msgbuf.data);
    return 0;
}
