
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/mysql_op.h"

#define MYSQL_DEFAULT_HOST  "127.0.0.1"
#define MYSQL_DEFAULT_PWD   "123"

/* -------------------------------------------*/
/**
 * @brief  打印操作数据库出错时的错误信息
 *
 * @param conn       (in)    连接数据库的句柄
 * @param title      (int)   用户错误提示信息
 *
 */
/* -------------------------------------------*/
void print_error(MYSQL *conn, const char *title)
{
    fprintf(stderr,"%s:\nError %u (%s)\n", title, mysql_errno(conn), mysql_error(conn));
}

/* -------------------------------------------*/
/**
 * @brief  连接数据库
 *
 * @param user_name	 (in)   数据库用户
 * @param passwd     (in)   数据库密码
 * @param db_name    (in)   数据库名称
 *
 * @returns
 *          成功：连接数据库的局部
 *			失败：NULL
 */
/* -------------------------------------------*/
MYSQL* mysql_m_conn(const char *user_name,const char* passwd,const char *db_name)
{
    MYSQL *conn = NULL; //MYSQL对象句柄


    //用来分配或者初始化一个MYSQL对象，用于连接mysql服务端
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql 初始化失败\n");
        return NULL;
    }
    //设置UTF8编码
    mysql_set_character_set(conn,"utf8");

    //mysql_real_connect()尝试与运行在主机上的MySQL数据库引擎建立连接
    //conn: 是已有MYSQL结构的地址。调用mysql_real_connect()之前，必须调用mysql_init()来初始化MYSQL结构。
    //NULL: 值必须是主机名或IP地址。如果值是NULL或字符串"localhost"，连接将被视为与本地主机的连接。
    //user_name: 用户的MySQL登录ID
    //passwd: 参数包含用户的密码
    if ( mysql_real_connect(conn,MYSQL_DEFAULT_HOST, user_name, passwd, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_conn 失败:Error %u(%s)\n", mysql_errno(conn), mysql_error(conn));

        mysql_close(conn);
        return NULL;
    }

    mysql_query(conn,"set character_set_results=utf8");

    return conn;
}


/* -------------------------------------------*/
/**
 * @brief  处理数据库查询结果
 *
 * @param conn	     (in)   连接数据库的句柄
 * @param res_set    (in)   数据库查询后的结果集
 *
 */
/* -------------------------------------------*/
void process_result_test(MYSQL *conn, MYSQL_RES *res_set)
{
    MYSQL_ROW row;
    uint i;

    // mysql_fetch_row从使用mysql_store_result得到的结果结构中提取一行，并把它放到一个行结构中。当数据用完或发生错误时返回NULL.
    while ((row = mysql_fetch_row(res_set)) != NULL)
    {

        //mysql_num_fields获取结果中列的个数
        for(i = 0; i < mysql_num_fields(res_set); i++)
        {
            if (i > 0)
            {
                fputc('\t',stdout);
            }
            printf("%s",row[i] != NULL ? row[i] : "NULL");
        }

        fputc('\n',stdout);
    }

    if( mysql_errno(conn) != 0 )
    {
        print_error(conn,"mysql_fetch_row() failed");
    }
    else
    {
        //mysql_num_rows接受由mysql_store_result返回的结果结构集，并返回结构集中的行数
        printf("%lu rows returned \n", (ulong)mysql_num_rows(res_set));
    }
}

//处理数据库查询结果，结果保存在buf，只处理一条记录
int process_result_one(MYSQL *conn, const char *sql_cmd, char *buf)
{
    if(sql_cmd == NULL)
    {
        print_error(conn,"sql_cmd is null\n");
        return -1;
    }

    if (mysql_query(conn, sql_cmd) != 0)
    {
        print_error(conn, "mysql_query error!\n");
        return -1;
    }

    MYSQL_RES *res_set;
    res_set = mysql_store_result(conn);/*生成结果集*/
    if (res_set == NULL)
    {
        print_error(conn, "smysql_store_result error!\n");
        return -1;
    }

    MYSQL_ROW row;
    uint i;
    ulong line = 0;

    if (mysql_errno(conn) != 0)
    {
        print_error(conn,"mysql_fetch_row() failed");
        return -1;
    }

    //mysql_num_rows接受由mysql_store_result返回的结果结构集，并返回结构集中的行数
    line = mysql_num_rows(res_set);
    if (line == 0)
    {
        return -1;
    }

    // mysql_fetch_row从结果结构中提取一行，并把它放到一个行结构中。当数据用完或发生错误时返回NULL.
    while ((row = mysql_fetch_row(res_set)) != NULL)
    {
        //mysql_num_fields获取结果中列的个数
        for (i = 0; i<mysql_num_fields(res_set); i++)
        {
            if (row[i] != NULL)
            {
                strcpy(buf, row[i]);
                return 0;
            }
        }
    }

    return -1;
}



//执行非查询类语句 返回影响的行数
my_ulonglong mysql_none_query(MYSQL *handle, char *sql)
{
    int ret=0;
    ret=mysql_query(handle,sql);
    if(ret!=0)
    {
        printf("msql_none_query error\n");
        return ret;
    }
    my_ulonglong rows=0;
    rows=mysql_affected_rows(handle);
    return rows;
}

//执行查询类语句
MYSQL_RES * mysql_m_query( MYSQL *handle, char *sql)
{
    int ret=0;
    mysql_query(handle,"set names utf8");
    ret=mysql_query(handle,sql);
    if(ret!=0)
    {
        printf("msql_none_query error\n");
        return NULL;
    }

    return mysql_store_result(handle);
}



