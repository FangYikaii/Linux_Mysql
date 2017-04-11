#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

int main()
{
    MYSQL conn_ptr;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row;
    char *query_str=NULL;
    int rc,i,fields;
    int rows;

    if(NULL==mysql_init(&conn_ptr))
    {
        printf("mysql_init():%s\n",mysql_error(&conn_ptr));
        return -1;
    }
    
    if(NULL==mysql_real_connect(&conn_ptr,"localhost","fyk_mysql",
                                "wishforme","child",0,NULL,0))
    {
        printf("mysql_real_connect():%s\n",mysql_error(&conn_ptr));
        return -1;
    }

    printf("1.Connected MYSQL Successful!\n");
    query_str="select * from children;";
    rc=mysql_real_query(&conn_ptr,query_str,strlen(query_str));
    if(0!=rc)
    {      
        printf("mysql_real_query():%s\n",mysql_error(&conn_ptr));
        return -1;
    }

    res=mysql_store_result(&conn_ptr);
    if(NULL==res)
    {
        printf("mysql_store_result():%s\n",mysql_error(&conn_ptr));
        return -1;
    }
    rows=mysql_num_rows(res);
    printf("The total rows is %d\n",rows);
    fields=mysql_num_fields(res);
    printf("The total fileds is %d\n",fields);
    while(row=mysql_fetch_row(res))
    {          
        for(i=0;i<fields;i++)
            printf("%s\t",row[i]);
        printf("\n");
    }
    mysql_close(&conn_ptr);
    return 0;
}
