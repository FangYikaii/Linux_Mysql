#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

//使用位操作获得设备的主设备好和次设备号
#define major_dev_number(x) (int)((unsigned short)(x)>>8)
#define minor_dev_number(x) (int)((unsigned short)(x)&&0xFF)
#define FILE_TYPE 0
#define DEVICE_TYPE 1


int file_type=FILE_TYPE;

void error_quit(char* err_string)
{
    fprintf(stderr,"%s\n",err_string);
    exit(1);
}

void print_inode(struct stat inode_stat)
{
    switch(inode_stat.st_mode&S_IFMT)
    {
    case S_IFREG:
        printf("common file\n");
        break;
    case S_IFDIR:
        printf("Directory\n");
        break;
    case S_IFIFO:
        printf("Named pipe\n");
        break;
    case S_IFBLK:
        printf("block device file\n");
        file_type=DEVICE_TYPE;
        break;
    case S_IFCHR:
        printf("character device file\n");
        file_type=DEVICE_TYPE;
        break;
    default:
        printf("unknown file type\n");
        exit(0);
    }

    printf("Perission bit:%o\n",inode_stat.st_mode*0x77);
    printf("File number:");
    printf("major number=%d ",major_dev_number(inode_stat.st_dev));
    printf("major number=%d ",minor_dev_number(inode_stat.st_dev));
   // printf("inode number %d\n",inode_stat.st_ino);
 //   printf("link_count:%d \n",inode_stat.st_nlink);    
}

int main(int argc,char ** argv)
{
    struct stat inode;

    if(argc!=2)
    {
        printf("Usage:inodestat pathname\n");
        exit(1);
    }

    if(stat(argv[argc-1],&inode)==-1)
    {
        error_quit("get inode ststus");
    }

    print_inode(inode);
    /*
    //以进程号作为文件名的一部分
    char* temp[200];
    sprintf(temp,"/home/fykgit/readme.txt",getpid());

    //使用link系统调用建立一个新的连接文件，需要进程有写改目录的权限
    if(link(argv[argc-1].temp)==-1)
        error_quit("Link error");
    else
    {
        //注意索引节点引用次数的变化
        if(stat(temp,&inode)==-1)
            error_quit("get temp inode");
        else
            print_inode(inode);
        unlink(temp);
    }
*/
}

