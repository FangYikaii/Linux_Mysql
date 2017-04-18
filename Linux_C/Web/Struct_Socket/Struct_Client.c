#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>    //包含套接字函数库
#include <netinet/in.h>    //包含AF_INET相关结构
#include <arpa/inet.h>     //包含AF_INET相关操作的函数
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8888
#define BUFFER_SIZE 1024

typedef struct
{
    int ab;
    int num[10000];
}Node;

int main()
{
    //sockfd
    int sock_cli=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT);
    server_addr.sin_addr.s_addr=inet_addr("192.168.5.109");

    if(connect(sock_cli,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        perror("connect:");
        exit(1);
    }

    //分配内存：Node
    Node* myNode=(Node*)malloc(sizeof(Node));
    myNode->ab=1231;
    myNode->num[0]=110;
    myNode->num[999]=333333;

    //分配内存：buffer
    int needSend=sizeof(Node);
    char* buffer=(char*)malloc(needSend);

    //内存块拷贝函数
    memcpy(buffer,myNode,needSend);

    int pos=0;
    int len=0;
    while(pos<needSend)
    {
        len=send(sock_cli,buffer+pos,BUFFER_SIZE,0);
        if(len<=0)
        {
            perror("ERRPR");
            break;
        }
        pos+=len;
    }
    free(buffer);
    free(myNode);
    close(sock_cli);
    printf("Send Over!\n");

    return 0;

}
