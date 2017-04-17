#include <stdio.h>
#include <unistd.h>

int main()
{
    //execl
    execl("/bin/ls","ls","-al","/etc/passwd",(char*)0);

    
    
    
    return 0;
}
