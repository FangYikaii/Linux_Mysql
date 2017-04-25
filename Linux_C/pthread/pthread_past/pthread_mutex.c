#include <stdio.h>   //standard input and output
#include <pthread.h>  //posix threads:for thread processing
#include <unistd.h>   //API for POSIX Operating System,C&c++
#include <stdlib.h>   //standard library

static int value=0;
pthread_mutex_t mutex;   //pthread_mutex_t:structure for mutex

void* func(void* args)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);   //lock the mutex,the thread was locked until unlock the mutex_lock
        sleep(1);
        value ++;
        printf("value=%d!\n",value);
        pthread_mutex_unlock(&mutex);   //unlock the mutex_lock
    }
}

int main()
{
    pthread_t pid1,pid2;   //pthead_t  structure for thread;  declaration two thread pid1 and pid2
    pthread_mutex_init(&mutex,NULL);  //initialize the mutex

    if(pthread_create(&pid1,NULL,func,NULL))  //CREATE thread pid1 for func,if success return 0 else return 1
    {
        return -1;
    }


    if(pthread_create(&pid2,NULL,func,NULL))  //CREATE thread pid2 for func,if success return 0 else return 1
    {
        return -1;
    }

    while(1)
    {
        sleep(0);
    }

    return 0;
}



