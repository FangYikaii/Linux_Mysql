#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

void error_quit(char* err_string)
{
    fprintf(stderr,"%s\n",err_string);
    exit(1);    //清楚进程的时钟超时标志，清楚进程可被跟踪的标志，清除进程的信号；如果是领导进程，则终止领导进程（SIGHUP）；关闭进程打开的所有文件，释放代码段，共享内存段，数据段，用户栈；将进程设置为僵尸状态
}


//子进程的标准输出和父进程的标准输入指向同一个管道文件，子进程的结果被父进程读入，实现管道功能
void main(int argc,char** argv)
{
    int fd[2];
    int fieldes;

    if(pipe(fd)==-1)
    {
        error_quit("create pipe");
    }

    switch(fork())
    {
        case -1:
            error_quit("fork child process");
        case 0:    //子进程
            close(1);     //1为标准输出函数，关闭标准输出函数
            dup(fd[1]);   //使用dup调用管道文件的写端进行拷贝，标准输出文件1将被指向管道的写端
            close(fd[1]); //关闭写描述符
            close(fd[0]); //关闭读描述符
            execlp("ls","ls","-l",NULL);         
            error_quit("try to exec ls");
            break;
        default:          //父进程
            close(0);     //0为标准输入函数，关闭标准输入函数
            dup(fd[0]);   //使用dup调用管道文件的读端进行拷贝，标准输入文件0将被指向管道的读端
            close(fd[0]);  //关闭读描述符
            close(fd[1]);  //关闭写描述符
            if((fieldes=open("exit.txt",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))<0) //打开txt，如果不存在则创建它
            {
                error_quit("open exit.txt error");
            }
            
            //重定向
            close(1);   //父进程关闭标准输出文件
            dup(fieldes);  //把新打开的文件的文件描述符复制到标准输出文件的描述符中
            close(fieldes);
            execlp("wc","wc","-l",NULL);  //父进程的输出被写到txt中
            error_quit("try to exec wc");
            break;
    }
}
