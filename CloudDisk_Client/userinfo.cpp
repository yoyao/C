#include "userinfo.h"


UserInfo::UserInfo()
{

}

int UserInfo::GetUid()
{
    return this->uid;
}

QString UserInfo::GetName()
{
    return this->name;
}
QString UserInfo::GetPasswd()
{

    return this->passwd;
}
int UserInfo::GetDeptid()
{

    return this->deptid;
}
int UserInfo::GetSex()
{

    return this->sex;
}
int UserInfo::GetAge()
{

    return this->age;
}
QString UserInfo::GetEmail()
{

    return this->email;
}
int  UserInfo::UserInfo::GetStat()
{
    return this->stat;
}
QDateTime UserInfo::GetRectime()
{
    return this->rectime;
}
QString  UserInfo::GetMark()
{
    return this->mark;
}

void UserInfo::SetUid(int uid)
{
  this->uid=uid;
}
void UserInfo::SetName(QString &name)
{
    this->name=name;
}
void UserInfo::SetPasswd(QString &pwd)
{
    this->passwd=pwd;
}
void UserInfo::SetDeptid(int did)
{
    this->deptid=did;
}
void UserInfo::SetSex(int sex)
{
    this->sex=sex;
}
void UserInfo::SetAge(int age)
{
    this->age = age;
}
void UserInfo::SetEmail(QString &email)
{
    this->email = email;
}
void UserInfo::SetStat(int stat)
{
    this->stat = stat;
}
void UserInfo::SetRectime(QDateTime &time)
{
    this->rectime = time;
}
void UserInfo::SetMark(QString &mark)
{
    this->mark = mark;
}

UserInfo UserInfo::FromJsonString(QJsonObject &obj)
{
    UserInfo user;
    QString tmp;
    user.SetAge(obj.value("age").toInt());
    user.SetDeptid(obj.value("deptid").toInt());
    tmp=obj.value("email").toString();
    user.SetEmail(tmp);
    //user.SetMark();
    tmp=obj.value("name").toString();
    user.SetName(tmp);
    tmp=obj.value("passwd").toString();
    user.SetPasswd(tmp);
    QDateTime cur=QDateTime::fromTime_t(obj.value("rectime").toInt());
    user.SetRectime(cur);
    user.SetSex(obj.value("sex").toInt());
    user.SetStat(obj.value("stat").toInt());
    user.SetUid(obj.value("uid").toInt());

    return user;
}
