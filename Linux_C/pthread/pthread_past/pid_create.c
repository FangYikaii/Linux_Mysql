#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    
    int count=0;
    pid=fork();

    if(-1==pid)
    {
        printf("Error happened in fork function!\n");
        return 0;
    }
    else if(0==pid)
    {
        printf("This is child process:%d\n",getpid());
        count++;
    }
    else
    {
        printf("This is parent process:%d\n",getpid());
        count++;
    }

    printf("Count:%d\n",count);
    return 0;
}
