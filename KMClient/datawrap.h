#ifndef DATAWRAP_H
#define DATAWRAP_H
#include "tcpclient.h"
#include <QObject>
#include <QString>

class DataWrap : public QObject
{
    Q_OBJECT
public:
    explicit DataWrap(QObject *parent = 0);
    QString sendData(QString loginName, QString pwd, QString operatorCmd, QString sql);//s
signals:

public slots:
private:
    QString  loginName;     //登录用户名
    QString  md5Pwd;       //用户MD5密码,长度为32位
    QString   operatorCmd;  //操作提示 A，代表增加，D代表删除等
    QString   sql;          //用于存取获取数据库字符操作数组



};

#endif // DATAWRAP_H
