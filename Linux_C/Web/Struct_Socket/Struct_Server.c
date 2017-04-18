#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8888
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024

typedef struct
{
    int ab;
    int num[10000];
}Node;

int main(int argc,char** argv)
{
    //set socket's address information
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htons(INADDR_ANY);
    server_addr.sin_port=htons(PORT);

    //create a stream socket
    int server_socket=socket(PF_INET,SOCK_STREAM,0);
    if(server_socket<0)
    {
        printf("create socket failed!\n");
        exit(1);
    }

    //bind
    if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("server bind port:%d Failed!\n",PORT);
        exit(1);
    }

    //listen
    if(listen(server_socket,LENGTH_OF_LISTEN_QUEUE))
    {
        printf("server listen failed!\n");
        exit(1);
    }

    while(1)
    {
        struct sockaddr_in client_addr;
        socklen_t length=sizeof(client_addr);

        int new_server_socket=accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if(new_server_socket<0)
        {
            printf("server accept failed!\n");
            break;
        }

        Node* myNode=(Node*)malloc(sizeof(Node));

        int needRecv=sizeof(Node);
        char* buffer=(char*)malloc(sizeof(Node));
        int pos=0;
        int len;
        while(pos<needRecv)
        {
            //recv(socket,接收数据的buffer地址，缓冲区长度，调用方式)：buffer+pos:接收数据的地址
            len=recv(new_server_socket,buffer+pos,BUFFER_SIZE,0);
            if(len<0)
            {
                printf("Server Receive Data Failed!\n");
                break;
            }
            pos+=len;
        }
        close(new_server_socket);
        //内存块拷贝函数
        //void* memcpy(void* destination,const void* source,size_t num);
        //从source的地址开始拷贝num个字节到destination开始的地址
        memcpy(myNode,buffer,needRecv);
        printf("recv over ab=%d num[0]=%d num[999]=%d\n",myNode->ab,myNode->num[0],myNode->num[999]);
        free(buffer);
        free(myNode);
    }
    close(server_socket);
    return 0;


}
