#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define FIFO_SERVER "./tmp/fifoserver"

int main()
{
    int fd,ret;                    //标志位：fd打开命名管道，ret创建命名管道
    char w_buf[1000];              //数组：写数据
    int w_num;                     //标志位：写数据（write）

    memset(w_buf,0,sizeof(w_buf));
    
    ret=mkfifo(FIFO_SERVER,O_CREAT|O_EXCL);   //创建命名管道（管道路径，属性）

    if(ret<0&&errno!=EEXIST)                   //如果创建错误
    {
        perror("create fifo error!\n");
        exit(-1);
    }

    printf("Create fifo done...\n");
    
    fd=open(FIFO_SERVER,O_WRONLY);             //打开文件
        
    if(fd==-1)                                 //打开文件失败
    { 
        perror("open file error!\n");
        exit(-1);
    }

    while(1)
    {
        printf("Please input:...\n");
        scanf("%s",w_buf);                          //输入写入数据
    
        w_num=write(fd,w_buf,strlen(w_buf));        //写入数据进w_buf

        if(w_num==-1&&errno!=EINTR)                 //如果写入数据错误
        {
            perror("write error!");
            exit(-1);
        }
        else if(w_num>0)                            //写入成功
        {
            printf("read write num is %d\n",w_num);
        }
    }
    return 0;

}
