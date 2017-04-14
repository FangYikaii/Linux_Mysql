#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int value=0;

void func(int sig)
{
    printf("I get a signal!\n");
    value =1;
}

int main()
{   
    signal(SIGINT,func);
    while(value==0)
    {
        sleep(1);
    }
    return 0;
}


