#include <stdio.h>
#include <sys/socket.h>   //Socket网络编程
#include <netinet/in.h>   //互联网地址族
#include <string.h>       //字符串处理头文件
#include <arpa/inet.h>    //信息转换，将客户端信息转换为字符串信息
#include <sys/un.h>       //

#define SERVERPORT 8888
#define MAXLEN     1000

int main()
{
    //监听，连接 文件描述
    int listenfd,connfd,n,i;
    //服务器和客户端的网络地址
    struct sockaddr_in serveraddr,clientaddr;
    //地址长度(客户端)
    socklen_t clientlen;
    //进程标识符
    pid_t childpid;
    //传送的数据
    char mesg[MAXLEN];

    //创建监听套接字
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    
    bzero(&serveraddr,sizeof(serveraddr));           //置字节字符串前n个字符为0，且包括'\n'   extern void bzero(void* s,int n);
    serveraddr.sin_family=AF_INET;                   //sin_familiy 协议簇，一般用AF_INET表示TCP/IP
    serveraddr.sin_port=htons(SERVERPORT);           //sin_port 端口号
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);    //表示本地任意地址的宏

    //绑定ip地址、端口号到要监听的套接字上
    bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

    //监听   int PASCAL FAR listen(SOCKET s,int backlog);  S:用于标识一个已捆绑未连接套接口的描述字 backlog:等待连接队列的最大长度
    listen(listenfd,1024);

    for(;;)   //可以有多个客户端和服务器同时连接，死循环
    {
        clientlen=sizeof(clientaddr);  //取客户端地址长度

        //父进程：用于创建连接，输入监听端口，返回一个新的链接套接字
        connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen);

        //子进程：以并发的方式处理客户端的信息
        if((childpid=fork())==0)
        {
            close(listenfd);     //关闭监听套接字端口

            for(;;)   //死循环，服务器端可以一直通讯
            {
                //接收客户端信息,返回接收信息的长度 recvfrom
                n=recvfrom(connfd,mesg,MAXLEN,0,(struct sockaddr*)&clientaddr,&clientlen);
                if(n==0)
                {
                    printf("the client %s is closed\n",inet_ntoa(clientaddr.sin_addr));
                    break;
                }
                printf("-------\n");
                printf("-------\n");
                mesg[n]='\0';
                printf("%s\n",mesg);    //打印客户端信息

                for(i=0;i<n;i++)
                {
                    mesg[i]=toupper(mesg[i]);    //toupper,将收到的小写字母转变成大写字母
                }
    
                //服务器发送信息  sendto
                sendto(connfd,mesg,n,0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));

            }

            close(connfd);   //关闭连接套接字端口
        }
    }
    
    close(listenfd);   //关闭监听套接字端口
    return 0;
}
