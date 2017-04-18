#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <crypt.h>
#include <sys/stat.h>
#include <fcntl.h>    //文件操作

#define BUFFER_SIZE 1024

int main(int argc,char* argv[])
{
    int from_fd,to_fd;                  //标志位：源文件，目标文件
    int bytes_read,bytes_write;         //标志位：读长度，写长度
    char buffer[BUFFER_SIZE];           //字符数组
    char *ptr;

    if(argc!=3)
    {
        fprintf(stderr,"Usage: %s fromfile to file\n\a",argv[0]);
        exit(1);
    }

    //打开源文件
    if((from_fd=open(argv[1],O_RDONLY))==-1)
    {
        fprintf(stderr,"Open %s Error:%s\n",*argv[1],strerror(errno));
        exit(1);
    }

    //创建目的文件
    if((to_fd=open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
    {
        fprintf(stderr,"Open %s Error:%s\n",*argv[2],strerror(errno));
        exit(1);
    }

    //经典拷贝代码：
    while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
    {
        if((bytes_read==-1)&&(errno!=EINTR))
            break;
        else if(bytes_read>0)
        {
            ptr=buffer;
            while(bytes_write=write(to_fd,ptr,bytes_read))
            {
                if((bytes_write==-1)&&(errno!=EINTR))
                    break;
                else if(bytes_write==bytes_read)
                    break;
                else if(bytes_write>0)
                {
                    ptr+=bytes_write;
                    bytes_read-=bytes_write;
                }
            }
            if(bytes_write==-1)
            {
                break;
            }
        
        }
    }
    close(from_fd);
    close(to_fd);
    exit(0);
}
