#include <stdio.h>   //standard input and output
#include <pthread.h>  //posix threads:for thread processing
#include <unistd.h>   //API for POSIX Operating System,C&c++
#include <stdlib.h>   //standard library


void* func_1(void* args)
{
    while(1)
    {
        sleep(1);
        printf("this is func_1\n");
    }
}


void* func_2(void* args)
{
    while(1)
    {
        sleep(1);
        printf("this is func_2\n");
    }
}

int main()
{
    pthread_t pid1,pid2;   //pthead_t  structure for thread;  declaration two threads pid1 and pid2

    if(pthread_create(&pid1,NULL,func_1,NULL))  //CREATE thread pid1 for func,if success return 0 else return 1
    {
        return -1;
    }


    if(pthread_create(&pid2,NULL,func_2,NULL))  //CREATE thread pid2 for func,if success return 0 else return 1
    {
        return -1;
    }

    while(1)
    {
        sleep(3);
    }

    return 0;
}



