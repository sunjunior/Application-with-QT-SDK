#include "tcpserver.h"
#include "tcpthread.h"
#include "tcpdialog.h"

TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
{
   dlg =(TcpDialog*)parent;

}
void TcpServer::incomingConnection(int socketDescriptor)
{
    TcpThread *thread = new TcpThread(socketDescriptor,this);
    connect(thread, SIGNAL(finished()), dlg, SLOT(showLog()),Qt::QueuedConnection);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()),Qt::DirectConnection);
    thread->start();
    thread->wait(); //等待子线程退出

}
