#include "qtelnet.h"
#include <QHostAddress>
#include <QStringList>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <QCoreApplication>
#include <QNetworkInterface>


QTelnet::QTelnet()
{


    m_pSocket = new QTcpSocket;

    output = "";
    isconnected = false;

}


void QTelnet::telnetToHost(QString serverAddr, QString portNo)
{
    m_serverAddr = serverAddr;
    m_portNo = portNo;
    slotstartConnection();
}


void QTelnet::stop()
{
    if(m_pSocket->state() == QAbstractSocket::ConnectedState)
    {
        m_pSocket->close();
    }

    output = "";
}





void QTelnet::slotstartConnection()
{
    if (m_serverAddr.isEmpty() || m_portNo.isEmpty())
    {
       qDebug()<<tr("服务器IP地址或端口为空，请确认输入后再试！") << endl;
       return;
    }
    qDebug()<<tr("连接中...") << endl;
    m_pSocket->abort();
    m_pSocket->connectToHost(QHostAddress(m_serverAddr), m_portNo.toInt());
    connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(slotdisplayError(QAbstractSocket::SocketError)));
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(slotreadMessage()));
    connect(m_pSocket, SIGNAL(connected()), this, SLOT(slotgetStatus()));
    connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(slotgetStatus()));


}

QTelnet::~QTelnet()
{
    if(m_pSocket)
    {
        m_pSocket->disconnectFromHost();
        m_pSocket->deleteLater();
    }
}

void QTelnet::slotgetStatus()
{

    switch(m_pSocket->state())
    {
    case QAbstractSocket::UnconnectedState:
    {
        qDebug() << tr("已断开连接！") << endl;
        isconnected = false;
        output = "";
        break;
    }

    case QAbstractSocket::ConnectingState:
    {
        qDebug() <<tr("正在连接...") << endl;
        isconnected = false;
        break;
    }

    case QAbstractSocket::ConnectedState:
    {
        qDebug() << tr("连接已建立！") << endl;
        isconnected = true;
        break;
    }

    default:
        break;

    }


}

void QTelnet::runCmds(QString cmds)
{
    this->cmds = cmds;
    slotsendMessage();
}

void QTelnet::runCmds(QFile *pfile)
{
    if (pfile == NULL)
    {
       qDebug()<< tr("默认脚本文件错误") << endl;
       return;
    }
    if (!pfile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<< tr("配置文件无法打开！") << endl;
        return;
    }
    QTextStream in(pfile);
    this->cmds = in.readAll();
    pfile->close();
    slotsendMessage();
}


void QTelnet::slotreadMessage()
{
    output += m_pSocket->readAll();
    qDebug()<< output << endl;

}


void QTelnet::slotsendMessage()
{
    if (cmds.isEmpty())
    {
        return;
    }
     qDebug()<<"send:" << cmds << endl;

    QStringList cmdList = cmds.split("\n");
    foreach(QString cmd, cmdList)
    {
        cmd += "\n";
        if (!m_pSocket->isValid())
        {
            return;
        }
        m_pSocket->write(cmd.toAscii());

       delaymsec(1000);

    }

}

void QTelnet::delaymsec(int msec)
{

    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }


}


void QTelnet::slotshowLog()
{
    QFile logfile("output.log");
    if(!logfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        return;
    }
    QTextStream out(&logfile);
    out << output;
    logfile.close();

}


void QTelnet::slotdisplayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
    {
        return;
    }
    isconnected = false;
    qDebug()<< tr("连接产生如下错误: %1.")
               .arg(m_pSocket->errorString()) <<endl;
    m_pSocket->close();
    output = "";

}


