#include "datawrap.h"
#include <QCryptographicHash>

DataWrap::DataWrap(QObject *parent) :
    QObject(parent)
{





}


QString DataWrap::sendData(QString loginName, QString pwd, QString operatorCmd, QString sql)
{
       //利用MD5加密
       QByteArray crypt;
       crypt = QCryptographicHash::hash (pwd.toAscii(), QCryptographicHash::Md5 );

       this->loginName = loginName;
       this->loginName.append("\n");
       this->md5Pwd = crypt.toHex();
       this->md5Pwd.append("\n");
       this->operatorCmd= operatorCmd;
       this->operatorCmd.append("\n"); //加入回车分割字符
       this->sql = sql;
       QString buffer;     //临时缓冲区
       buffer = this->loginName+this->md5Pwd+this->operatorCmd+this->sql;


       return buffer;       //返回buffer

}
