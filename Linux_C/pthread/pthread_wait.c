#include <stdio.h>   //standard input and output
#include <pthread.h>  //posix threads:for thread processing
#include <unistd.h>   //API for POSIX Operating System,C&c++
#include <stdlib.h>   //standard library


void* func(void* args)
{
    sleep(2);
    printf("this is func_wait!\n");
}

int main()
{
    pthread_t pid;   //pthead_t  structure for thread;  declaration thread pid

    if(pthread_create(&pid,NULL,func,NULL))  //CREATE thread pid1 for func,if success return 0 else return 1
    {
        return -1;
    }

    pthread_join(pid,NULL);   //pthread_joid: function main wait, until thread pid end

    printf("this is end of main!\n");

    return 0;
}



