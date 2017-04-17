#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>  //网络地址族
#include <string.h>
#include <stdlib.h>
#include <errno.h>       //定义错误码来回报错误的宏

#define MAXLEN 1000


int main(int argc,char ** argv)
{
    int sockfd;    //套接字描述符
    int n;

    //套接字信息的数据结构：服务器地址
    struct sockaddr_in serveraddr;
    //host entry的缩写，记录主机的信息：主机名、别名、地址类型、地址长度、地址列表
  // struct hostent* host=NULL;

    char sendline[MAXLEN];
    char receline[MAXLEN];


    if(argc!=3)
    {
        printf("usage:./Client.c  <ip address> <ip port>\n");
        exit(1);
    }
    /*
    ptr=argv[1];
    host=gethostbyname(ptr);                      //主机信息
    if(host==NULL)
    {
        fprintf(stderr,"gethostbyname error \n\a");
        exit(0);
    }
    */
    
    sockfd=socket(AF_INET,SOCK_STREAM,0);              //创建客户端套接字

    if(sockfd<0)
    {
        printf("Create socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
    }
    
    bzero(&serveraddr,sizeof(serveraddr));            //清空结构体
    serveraddr.sin_family=AF_INET;                     //协议类型IPV4

    serveraddr.sin_port=htons(atoi(argv[2]));                 //端口号（字节序转换）  利用atoi：字符串转整型
    serveraddr.sin_addr.s_addr=inet_addr(argv[1]);     //IP地址（地址格式化）

    if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
    {
        printf("Connnect error:%s(error no: %d)\n",strerror(errno),errno);
        exit(0);
    }

    while(fgets(sendline,MAXLEN,stdin)!=NULL)    //读键盘输入内容，到sendline所指向的内存空间
    {
        //sendto 传送数据到服务器
        sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

        //recvfrom 从服务器接收数据
        n=recvfrom(sockfd,receline,MAXLEN,0,NULL,NULL);

        //字符串结束的标志
        receline[n]='\n';

        //显示收到的数据
        fputs(receline,stdout);
    }

    close(sockfd);

    return 0;

}

