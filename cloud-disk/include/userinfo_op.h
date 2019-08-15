#ifndef USERINFO_OP_H
#define USERINFO_OP_H

#include <time.h>
#include "mysql_op.h"

typedef struct _userinfo
{
    int    uid;//用户的唯一标识符
    char   name[64];//用户名
    char   passwd[32];//密码
    int    deptid;//部门ID
    short  sex;//0不详 1男 2女
    int    age;//
    char   email[64];//
    time_t rectime;//注册时间
    int    stat;//状态 0正常 1冻结 2删除等
    char   *mark;//备注
} UserInfo;


int user_get_by_account_pwd(MYSQL *handle, UserInfo *user, const char *account, const char *pwd);
int user_insert(UserInfo *user);
int user_get_byid(int uid,UserInfo *user);

int user_update(UserInfo *user);









#endif // USERINFO_OP_H
