#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class TcpDialog;


class TcpServer : public QTcpServer
{

  Q_OBJECT

public:
    explicit TcpServer(QObject *parent = 0);
protected:
    void incomingConnection(int socketDescriptor);
private:
                TcpDialog *dlg;


};

#endif // TCPSERVER_H
