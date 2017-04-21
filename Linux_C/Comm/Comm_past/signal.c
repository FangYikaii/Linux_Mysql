#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void func(int sign_no)
{
    if(sign_no==SIGINT)
    {
        printf("I have get SIGINT\n");
    }
    else if(sign_no==SIGQUIT)
    {
        printf("I have get SIGQUIT\n");
    }
}

int main()
{
    printf("Waiting for SIGINT or SIGQUIT\n");
    signal(SIGINT,func);

    signal(SIGQUIT,func);
    pause();   
    return 0;
}


