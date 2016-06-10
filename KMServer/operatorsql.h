#ifndef OPERATORSQL_H
#define OPERATORSQL_H

#include <QObject>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QStringList>

class OperatorSql : public QObject
{
    Q_OBJECT
public:
    explicit OperatorSql(QObject *parent = 0);
    ~OperatorSql();
    bool verifyUser(const QString name, const QString passwd);//认证用户姓名密码
    bool createConnection(); //数据库连接
    bool addRecode(QString sql); //数据库记录增加
    bool deleteRecode(QString sql);//数据库记录删除
    bool updateRecode(QString sql);//数据库记录修改
    bool queryRecode(QString sql); //数据库记录查询


 private:
    QSqlDatabase db; //创建数据库句柄
    QStringList sqlstr;//

};

#endif // OPERATORSQL_H
