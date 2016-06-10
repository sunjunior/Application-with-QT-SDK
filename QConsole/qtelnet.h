#ifndef QTELNET_H
#define QTELNET_H

#include <QtNetwork/QTcpSocket>
class QTimer;
class QFile;

class QTelnet:public QObject
{
    Q_OBJECT
public:
     QTelnet();//用户名，密码，设备IP，端口23
    ~QTelnet();
     void telnetToHost(QString serverAddr, QString portNo = 23);
     void stop();
     void runCmds(QString cmds);//执行命令
     void runCmds(QFile *pfile); //加载默认脚本
     bool isConnected() const { return  isconnected;} //返回连接是否成功
private slots:
    void slotreadMessage();
    void slotsendMessage();
    void slotshowLog();
    void slotgetStatus();
    void slotstartConnection(void);
    void slotdisplayError(QAbstractSocket::SocketError socketError);

private:
    void delaymsec(int msec);



private:
    QTcpSocket *m_pSocket;
    QString output;
    QString m_username;
    QString m_password;
    QString m_serverAddr;
    QString m_portNo;

    bool isconnected;
    bool loginflag;

    QTimer *loginTimer;
    QString cmds;
    QStringList filesList;


};

#endif // QTELNET_H
