#ifndef USERINFO_H
#define USERINFO_H

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

class UserInfo
{
public:
    UserInfo();

   int  GetUid();
   QString GetName();
   QString GetPasswd();
   int    GetDeptid();
   int    GetSex();
   int    GetAge();
   QString GetEmail();
   int     GetStat();
   QDateTime GetRectime();
   QString   GetMark();

   void SetUid(int uid);
   void SetName(QString &name);
   void SetPasswd(QString &pwd);
   void SetDeptid(int did);
   void SetSex(int sex);
   void SetAge(int age);
   void SetEmail(QString &email);
   void SetStat(int stat);
   void SetRectime(QDateTime &time);
   void SetMark(QString &mark);

private:
    int uid;
    QString name;
    QString passwd;
    int deptid;
    int sex;
    int age;
    QString email;
    int stat;
    QDateTime rectime;
    QString mark;
public:
    static UserInfo FromJsonString(QJsonObject &obj);
};

#endif // USERINFO_H
