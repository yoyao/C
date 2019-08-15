#include "include/userinfo_op.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_USER_    "root"
#define DB_PWD_     "123"
#define DB_DB_NAME_ "clouddisk"

void user_copy_attr(UserInfo *p_file,const char *field,const char *attr);
void printfuser(UserInfo *user);

int mainxxxx(int argc, char *argv[])
{
    UserInfo user;
    bzero(&user,sizeof(user));
    MYSQL *mysql_handle=NULL;
    mysql_handle=mysql_m_conn(DB_USER_, DB_PWD_,DB_DB_NAME_);
    if(mysql_handle==NULL)
    {
        puts("mysql_m_conn error\n");
        return 0;
    }
    user_get_by_account_pwd(mysql_handle,&user,"sanji","111");
    printfuser(&user);



    return 0;
}


int user_get_by_account_pwd(MYSQL *handle,UserInfo *user,const char *account,const char *pwd)
{
    bzero(user,sizeof(*user));
    int ret=0;
    char sql[128]={0};
    sprintf(sql,"SELECT * FROM userinfo where name='%s' and passwd='%s' ",account,pwd);

    MYSQL_RES *p_res=NULL;
    MYSQL_FIELD *fields=NULL;
    do
    {
        p_res=mysql_m_query(handle,sql);
        if(p_res==NULL)
        {
            ret=-1;
            break;
        }
        fields=mysql_fetch_fields(p_res);
        //获得的所有的属于这个用户的文件的个数
        unsigned int fleng= mysql_num_fields(p_res);
        if(fleng <1)
        {
            ret= 1;
            break;
        }
        unsigned int rowcount=mysql_num_rows(p_res);
        if(rowcount !=1)
        {
            ret= 2;
            break;
        }

        int i;
        for(i=0;i<rowcount;++i)
        {
            MYSQL_ROW row=NULL;
            row=mysql_fetch_row(p_res);
            int j;
            for(j=0;j<fleng;++j)
            {
                user_copy_attr(user,fields[j].name,row[j]);
            }
        }

    }while(0);

    if(p_res !=NULL)
    {
        mysql_free_result(p_res);
    }
    return ret;
}

int user_insert(UserInfo *user)
{
    int ret=-1;



    return ret;
}
int user_get_byid(int uid,UserInfo *user)
{
    int ret=-1;
bzero(user,sizeof(*user));


    return ret;
}
int user_update(UserInfo *user)
{
    int ret=-1;



    return ret;
}

void user_copy_attr(UserInfo *p_user,const char *field,const char *attr)
{
    if(p_user==NULL||field==NULL||attr==NULL)
    {
        return;
    }
    printf("field %s  attr %s\n",field,attr);
    /*
uid,name,passwd,deptid,sex,age,email,stat,rectime,mark
*/
    size_t len=strlen(attr);

    if(strcmp(field,"uid")==0)
    {
        p_user->uid=atoi(attr);
        return ;
    }
    if(strcmp(field,"age")==0)
    {
        p_user->age=atoi(attr);
        return ;
    }
    if(strcmp(field,"email")==0)
    {
        memcpy(p_user->email,attr,len);
        return ;
    }

    if(strcmp(field,"deptid")==0)
    {
        p_user->deptid=atoi(attr);
        return ;
    }

    if(strcmp(field,"name")==0)
    {
        memcpy(p_user->name,attr,len);
        return ;
    }

    if(strcmp(field,"passwd")==0)
    {
        memcpy(p_user->passwd,attr,len);
        return ;
    }

    if(strcmp(field,"rectime")==0)
    {
        char timeb[64]={0};
        struct tm cur;
        bzero(&cur,sizeof(cur));
        strptime(attr,"%Y-%M-%d %H:%M:%S",&cur);
        p_user->rectime=mktime(&cur);
        return ;
    }

    if(strcmp(field,"sex")==0)
    {
        p_user->sex=atoi(attr);
        return ;
    }

    if(strcmp(field,"stat")==0)
    {
        p_user->stat=atoi(attr);
        return ;
    }
    /*
    if(strcmp(field,"mark")==0)
    {
        memcpy(p_user->mark,attr,len);
        return ;
    }
    */
}

void printfuser(UserInfo *user)
{
    printf("start print user\n");
    printf("uid:%d\n",user->uid);
    printf("name:%s\n",user->name);
    printf("passwd:%s\n",user->passwd);
    printf("deptid:%d\n",user->deptid);
    printf("sex:%d\n",user->sex);
    printf("age:%d\n",user->age);
    printf("email:%s\n",user->email);
    printf("stat:%d\n",user->stat);
    printf("rectime:%ld\n",user->rectime);
}
