#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "message.h"

static int msg_que_id=-1;             //消息队列ID
static ticket_number=1000;            //剩余票数

void catch_signal_term(int signo)     //action函数：退出ser_list程序，删除message_list
{
    if(semctl(msg_que_id,0,IPC_RMID,0)==-1) //信号量控制：semctl；IPC_RMID：删除对应的信号量
    {
        perror("remove message list");
    }
    else
    {
        printf("remove message list\n");
    }
    printf("message list server exit.\n");
    exit(1);
}

void main(int argc,char** argv)
{
    Message rece_msg,send_msg;          //发送信息，接受信息结构体
    int rece_len,send_len,ret;          //接收信息长度，发送信息长度，接受信息标志位，
    struct sigaction action;            //sigaction:信号处理函数结构体
    int ticket_want;                    //想要的票数

    msg_que_id=msgget(BOOKING_KEY,0);   //消息队列ID
    if(msg_que_id!=-1)
    {
        if(msgctl(msg_que_id,IPC_RMID,0)==-1)
        {
            perror("remove old message list");
            exit(1);
        }
    }
    printf("remove old message list");

    //创建消息队列
    msg_que_id=msgget(BOOKING_KEY,IPC_CREAT|0666);
    if(msg_que_id==-1)
    {
        perror("create new message list");
        exit(1);
    }
    printf("create new message list");

    //安装信号处理器，当进程收到SIGTERM信号的时候，将删除消息队列并退出
    action.sa_handler=catch_signal_term;     //action的函数指针
    sigemptyset(&action.sa_mask);            //屏蔽信号集合设置为空
    action.sa_flags=0;                       //标志设置为0
    if(sigaction(SIGTERM,&action,NULL)==-1)
        perror("can not install the handler for SIGTERM");

    //忽略其他信号:SIG_IGN
    action.sa_handler=SIG_IGN;     //SIG_DEL:默认信号处理程序；SIG_IGN:忽略信号处理程序
    action.sa_flags=0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT,&action,NULL);   //进程中断：ctrl+c
    sigaction(SIGQUIT,&action,NULL);  //进程退出
    sigaction(SIGHUP,&action,NULL);   //用户从shelllogout

    while(1)
    {
        //接收所有标志为CLIENT_INPUT的消息
        rece_len=msgrcv(msg_que_id,&rece_msg,sizeof(Message)-sizeof(long),CLIENT_INPUT,0);
        if(rece_len<0)
        {
            perror("receive message");
            raise(SIGTERM);   //raise:向执行的程序发送一个信号;SIGTERM:软件终止信号
        }
        
        //填写回应的标志
        send_msg.m_type=rece_msg.process_id;
        send_msg.process_id=getpid();
        ticket_want=rece_msg.m_buffer[0];
        printf("receive a request for %d ticket left.\n",ticket_want);
        if(ticket_number>=ticket_want&&ticket_number>0)
        {
            send_msg.m_buffer[0]=1;
            ticket_number-=ticket_want;
            printf("There are %d tickets left now.\n",ticket_number);
        }
        else
        {
            send_msg.m_buffer[0]=0;
        }

        send_len=sizeof(long)+sizeof(int);       
        ret=msgsnd(msg_que_id,&send_msg,send_len,0);

        if(ret<0)
        {
            perror("server and message");
            raise(SIGTERM);
        }
    }



}


