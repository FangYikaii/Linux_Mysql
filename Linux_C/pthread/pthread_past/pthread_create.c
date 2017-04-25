#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREAD 8

void* printHello(void *args)
{
    int thread_arg;
    sleep(1);
    thread_arg=(int)(*((int*)args));
    printf("Hello from thread %d\n",thread_arg);
    return NULL;
}

int main()
{
    int rc,t;
    pthread_t thread[NUM_THREAD];


    for(t=0;t<NUM_THREAD;t++)
    {
        printf("Creating thread %d\n",t);
        rc=pthread_create(&thread[t],NULL,printHello,&t);
        if(rc)
        {
            printf("ERROR:return code is %d\n",rc);
            return EXIT_FAILURE;
        }
    }
    sleep(5);
    for(t=0;t<NUM_THREAD;t++)
    {
        pthread_join(thread[t],NULL);
    }
    return EXIT_SUCCESS;
}
