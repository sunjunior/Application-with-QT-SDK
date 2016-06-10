#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QStringList>
#include <QTimer>

class TcpThread : public QThread
{
    Q_OBJECT
public:
   TcpThread(int socketDescriptor, QObject *parent);
   ~TcpThread();

    void run();                                      //子线程启动程序
   void opeatorData();                             //还可以设置为返回值QString型，获取操作成功提示
public slots:
   void getClientData(); //接受客户端数据

   void saveLog();              //保存日志

   void sendMsg(const QString message);  //发送成功或者失败消息
signals:
    void error(QTcpSocket::SocketError socketError); //错误提示


private:
    int socketDescriptor;

    QTcpSocket *tcpSocket; //服务器连接上后与客户端通讯的子套接字

    qint16  blocksize;//接收到的数据字节数大小
    QByteArray inBlock;//接受的消息数据

    QString data[4];     //用于存取发送过来的每一个字段
    QStringList list;       //将接收到的字符串按照指定符号分割,生成发送过来的字段字符串


    QTimer *timer;
};

#endif // TCPTHREAD_H
