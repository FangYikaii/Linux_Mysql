#include <sys/types.h>   
#include <sys/socket.h>   
#include <stdio.h>   
#include <memory.h>
#include <netinet/in.h>   
#include <arpa/inet.h>  
#include <unistd.h>  
#include <stdlib.h>  
     
#define BUFFER_SIZE 40  
      
int main()   
{         
    char buf[BUFFER_SIZE];  
    int server_sockfd, client_sockfd;   
    int sin_size=sizeof(struct sockaddr_in);   
    struct sockaddr_in server_address;   
    struct sockaddr_in client_address;   
    memset(&server_address,0,sizeof(server_address));  
    server_address.sin_family = AF_INET;   
    server_address.sin_addr.s_addr = INADDR_ANY;   
    server_address.sin_port = htons(8887);   
    // 建立服务器端socket   
    if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)  
    {  
        perror("server_sockfd creation failed");  
        exit(EXIT_FAILURE);  
    }  
    // 设置套接字选项避免地址使用错误
    int on=1;  
    
    if((setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
            perror("setsockopt failed");  
            exit(EXIT_FAILURE);  
    }  
    if((bind(server_sockfd,(struct sockaddr *)&server_address,sizeof(struct sockaddr)))<0)
    {
        perror("server socket bind failed");  
        exit(EXIT_FAILURE);  
     } 
     while(1)
     {        
         // 将套接字绑定到服务器的网络地址上

        // 建立监听队列  
        listen(server_sockfd,5);  
        // 等待客户端连接请求到达  
        client_sockfd=accept(server_sockfd,(struct sockaddr *)&client_address,(socklen_t*)&sin_size);  
        if(client_sockfd<0)  
        {  
            perror("accept client socket failed");  
            exit(EXIT_FAILURE);  
        }  
        // 接收客户端数据  
        if(recv(client_sockfd,buf,BUFFER_SIZE,0)<0)  
        {  
            perror("recv client data failed");  
            exit(EXIT_FAILURE);  
        }  
        printf("receive from client:%s/n",buf);  
        // 发送数据到客户端  
        if(send(client_sockfd,"I have received your message.",BUFFER_SIZE,0)<0)  
        {  
            perror("send failed");  
            exit(EXIT_FAILURE);  
        }
      }
      close(client_sockfd);  
      close(server_sockfd);  
      exit(EXIT_SUCCESS);   
}  
