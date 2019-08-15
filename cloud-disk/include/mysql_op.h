#ifndef MYSQL_OP_H
#define MYSQL_OP_H

#include <mysql/mysql.h> //数据库


/**
 * @brief  打印操作数据库出错时的错误信息
 *
 * @param conn       (in)    连接数据库的句柄
 * @param title      (int)   用户错误提示信息
 *
 */
void print_error(MYSQL *conn, const char *title);

/**
 * @brief  连接数据库
 *
 * @param user_name	 (in)   数据库用户
 * @param passwd     (in)   数据库密码
 * @param db_name    (in)   数据库名称
 *
 * @returns
 *          成功：连接数据库的句柄
 *			失败：NULL
 */
MYSQL* mysql_m_conn(const char *user_name,const char* passwd,const char *db_name);

/**
 * @brief  处理数据库查询结果
 *
 * @param conn	     (in)   连接数据库的句柄
 * @param res_set    (in)   数据库查询后的结果集
 *
 */
void process_result_test(MYSQL *conn, MYSQL_RES *res_set);

//处理数据库查询结果，结果保存在buf，只处理一条记录
int process_result_one(MYSQL *conn, const  char *sql_cmd, char *buf);


//执行查询类语句
MYSQL_RES * mysql_m_query(MYSQL *handle, char *sql);
//执行非查询类语句
my_ulonglong mysql_none_query( MYSQL *handle,char *sql);



#endif // MYSQL_OP_H
