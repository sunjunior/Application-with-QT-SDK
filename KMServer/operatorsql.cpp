#include "operatorsql.h"
#include <QtGui>
#include <QtSql/QSqlError>


OperatorSql::OperatorSql(QObject *parent):QObject(parent)
{
      QSqlDatabase::database("qt_sql_default_connection", false).close();//关闭默认数据库连接
      QSqlDatabase::removeDatabase("qt_sql_default_connection"); //移除默认的数据库名连接

}




bool OperatorSql::createConnection()
{

    //  db = QSqlDatabase::addDatabase("QSQLITE");//加载数据库驱动
        db = QSqlDatabase::addDatabase("QMYSQL");
     //db.setHostName("202.118.69.105");
        db.setHostName("192.168.0.3");
        db.setUserName("root");
        db.setPassword("");

    //db.setDatabaseName("./Database/CMedicalDB"); //设置数据库文件名
       db.setDatabaseName("CMedicalDB"); //设置数据库文件名
         if(!db.open())
         {
             qDebug()<< tr(" 不能打开数据库文件");
                 return false;
         }

           return true;

}

bool OperatorSql::verifyUser(const QString name,const QString passwd)
{

//  QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));//指定数据库查询编码格式GBK
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));//指定QString类的编码格式
 QString sqlstr = "select * from  UsersTable where LoginName = '"  + name + "' and Password ='" + passwd + "'";//建立一条数据库查询语句



 qDebug()<<tr("%1").arg(sqlstr);

QSqlQuery query; //操作数据库




   query.exec(QObject::tr("%1").arg(sqlstr));
   if(!query.isActive())
   {
    qDebug()<<query.lastError().text();
    return false;
   }



   if(query.next())
   {
       qDebug() << tr("%1").arg(query.value(0).toString())<<endl;
        return true;
   }



    return false;

}

bool OperatorSql::queryRecode(QString sql)
{

    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
      {     //先判断该数据库是否支持事务操作
          QSqlQuery query;
          if(QSqlDatabase::database().transaction()) //启动事务操作
          {
              //
              //下面执行各种数据库操作
              query.exec(sql);

              //
              if(!QSqlDatabase::database().commit())
              {
                  qDebug() << QSqlDatabase::database().lastError(); //提交
                  if(!QSqlDatabase::database().rollback())
                      qDebug() << QSqlDatabase::database().lastError(); //回滚
              }
          }
          //输出整张表
          query.exec("select * from UserTable;");
          while(query.next())
              qDebug() << query.value(1).toString() << query.value(2).toString();
      }

    return true;



}
bool OperatorSql::updateRecode(QString sql)
{

  QSqlQuery query;
  sqlstr = sql.split(";"); //用分号作为分隔
//  QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));//指定数据库查询编码格式GBK
 QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));//指定QString类的编码格式

  QString str;

  int count = 0; //用来记录数据库操作记录数

  foreach(str,sqlstr)
  {


if(!query.exec(str))
{
 QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));//指定Tr的编码格式
 qDebug()<<query.lastError().text();
 return false;
}
else
   {
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//指定Tr的编码格式
    qDebug()<<tr("第 %1 条操作结果：修改操作成功！").arg(++count);


  }

  }
  return true;

}

bool OperatorSql::addRecode(QString sql)
{

    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));//指定数据库查询编码格式GBK
   QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));//指定QString类的编码格式
    sqlstr = sql.split(";"); //用分号作为分隔

    QSqlQuery query;
    QString str;
    int count = 0; //用来记录数据库操作记录数
    foreach(str,sqlstr)
    {

  if(!query.exec(str))
  {
   QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));//指定Tr的编码格式
      qDebug()<<query.lastError().text();
   return false;
  }
  else
     {
      QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//指定Tr的编码格式
      qDebug()<<tr("第 %1 条操作结果：添加操作成功！").arg(++count);
     }

    }
    return true;
}

bool OperatorSql::deleteRecode(QString sql)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));//指定数据库查询编码格式GBK
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));//指定QString类的编码格式
  QSqlQuery query;
  sqlstr = sql.split(";"); //用分号作为分隔


  QString str;
  int count = 0; //用来记录数据库操作记录数

  foreach(str,sqlstr)
  {

if(!query.exec(str))
{
 qDebug()<<query.lastError().text();
 return false;
}
else
{
      QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//指定Tr的编码格式
     qDebug()<<tr("第 %1 条操作结果：删除操作成功！").arg(++count);

}

  }
  return true;

}

OperatorSql::~OperatorSql()
{
 db.close();
}

