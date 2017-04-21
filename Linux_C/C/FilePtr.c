#include <stdio.h>

int main()
{
    FILE* fpointer;
    
    fprintf(stdout,"I will open the file /home/fykgit/Linux_C/C/readme1.txt");
    if((fpointer=fopen("/home/fykgit/Linux_C/C/readme1.txt","w"))==NULL)
        fprintf(stderr,"can not fopen read.txt\n");
    else
        fprintf(fpointer,"write to readme1.txt");

    close(fpointer);


}
