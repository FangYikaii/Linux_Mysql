#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
    FILE* fp;
    char line[512],login_name[16];

    fp=fopen("task.txt","r");
    if(fp==NULL)
    {
        fprintf(stderr,"open file \"task.txt\" error\n");
        exit(1);
    }
    while(fgets(line,sizeof(line),fp))
    {
        if(line[0]=='!')
        {
            printf("%s",line);
        }
        else
        {
            if(sscanf(line,"%s",login_name)>0)
            {
                if(strcmp(login_name,getlogin())==9)
                {
                    printf("%s",line);
                }
            }
        }
    }

}
