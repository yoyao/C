#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>

#define M_MYSQL_HOST     "192.168.82.25"
#define M_MYSQL_USER     "root"
#define M_MYSQL_PWD      "123"
#define M_MYSQL_DB_NAME  "clouddisk"
#define M_MYSQL_PORT     0


int main(int argc, char *argv[])
{
    MYSQL *p_handle;
    int ret=-1;
    p_handle= mysql_init(NULL);
    if(p_handle ==NULL)
    {
        printf("mysql_init error %s\n",mysql_errno(p_handle));
        return 0;
    }

    p_handle=mysql_real_connect(p_handle,M_MYSQL_HOST,M_MYSQL_USER,M_MYSQL_PWD,M_MYSQL_DB_NAME,M_MYSQL_PORT,NULL,0);
    if(p_handle ==NULL)
    {
        printf("mysql_real_connect error %s\n", mysql_errno(p_handle));
        return 0;
    }
    char sql[128]={"select * from userinfo"};
    ret=mysql_query(p_handle,sql);
    if(ret<0)
    {
        printf("mysql_query terror\n");
        return 0;
    }
    MYSQL_RES *p_result=NULL;
    MYSQL_FIELD *p_fields=NULL;
    p_result=mysql_store_result(p_handle);
    if(p_result==NULL)
    {
        printf("mysql_store_result error %s\n", mysql_errno(p_handle));
        return 0;
    }
    p_fields=mysql_fetch_fields(p_result);
    if(p_fields==NULL)
    {
        printf("mysql_fetch_fields error %s\n", mysql_errno(p_handle));
        return 0;
    }

    ret=mysql_field_count(p_handle);
    MYSQL_ROW row=  mysql_fetch_row(p_result);
    if(p_fields==NULL)
    {
        printf("mysql_fetch_fields error %s\n", mysql_errno(p_handle));
        return 0;
    }
    printf("start fetch row\n");
    int i=0;
    for(i=0;i<ret;++i)
    {
       printf("%s \n",row[i]);
    }

    mysql_free_result(p_result);
    mysql_close(p_handle);
    return 0;
}
