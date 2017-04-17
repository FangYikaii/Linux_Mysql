#include <stdio.h>      //标准输入输出
#include <stdlib.h>     //标准库函数 
#include <sys/stat.h>   //读取文件状态
#include <fcntl.h>     //文件处理 
#include <errno.h>    //错误输出
#include <string.h>   //字符串处理

#define FIFO_SERVER "./tmp/fifoserver"

int main()
{
    int fd;               //标志位:命名管道打开
    char r_buf[1000];     //数组：buffer
    int r_num;            //标志位：fifo读取（read）

    memset(r_buf,0,sizeof(r_buf));  //r_buf所指向的内存全都设置为0

    fd=open(FIFO_SERVER,O_RDONLY);       //打开fifo文件
    if(fd==-1)                            //如果打开失败
    {
        perror("open fifo error!/n");
        exit(-1);
    }

    while(1)
    {
        sleep(2);
        memset(r_buf,0,sizeof(r_buf));  //r_buf所指向的内存全都设置为0
        r_num=read(fd,r_buf,1000);         //r_num:返回读取数组的长度
        if(r_num==-1&&errno!=EINTR)        //如果读fifo文件失败
        {
            perror("read error!\n");
            exit(-1);
        }
        r_buf[r_num]='\0';                //添加字符串结束符号
        printf("real read byte:%d,is: %s\n",r_num,r_buf);
    }
    unlink(FIFO_SERVER);

    return 0;
}
