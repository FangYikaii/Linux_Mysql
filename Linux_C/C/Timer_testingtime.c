#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

//算法分析
void function()
{
    unsigned int i,j;
    double y;
    for(i=0;i<1000;i++)
    {
        for(j=0;i<1000;i++)
        {
            y++;
        }
    }
}

int main()
{
    struct timeval tpstart,tpend;
    float timeuse;

    gettimeofday(&tpstart,NULL);  //开始时间
    function();
    gettimeofday(&tpend,NULL);    //结束时间

    //计算执行时间
    timeuse =1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;
    printf("Used time:%f",timeuse);
    exit(0);
}
