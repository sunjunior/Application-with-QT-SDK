#include "tcpdialog.h"
#include <QtGui>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostInfo>

#include <QNetworkInterface>
#include <QList>

#include "tcpserver.h"

TcpDialog::TcpDialog(QWidget *parent)
    : QDialog(parent),count(0)

{
    server = new TcpServer(this);

    serverStatusLabel = new QLabel(tr("服务端就绪"));

    startButton = new QPushButton(tr("启动服务"));
    quitButton = new QPushButton(tr("退出"));
    reqStatusLable = new QLabel;
    serverIpLabel = new QLabel(tr("监听IP地址："));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startlisten()));//监听端口服务
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));


    QVBoxLayout *mainLayout = new QVBoxLayout;


    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(serverIpLabel);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(reqStatusLable);
    setLayout(mainLayout);
    setFixedSize(480,360);

    setWindowTitle(tr("服务器程序"));
    setWindowIcon(QIcon(":icon/server.ico"));

  }

void TcpDialog::startlisten()
{

#ifdef Q_OS_LINUX

    QNetworkInterface  localInterface =localInterface.interfaceFromName("eth0"); //设备名称为eth0
    QString localIP = localInterface.addressEntries().at(0).ip().toString();

     qDebug()<< localIP;

     while (!server->isListening() && !server->listen(QHostAddress(localIP),5500))
     {
             QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                             tr("连接测试"),
                                             tr("无法进行连接: %1.")
                                             .arg(server->errorString()),
                                             QMessageBox::Retry
                                             | QMessageBox::Cancel);
             if (ret == QMessageBox::Cancel)
                  return;

         }

serverIpLabel->setText(tr("监听IPv4地址：%1").arg(localIP));

#endif



#ifdef  Q_OS_WIN32

    QString localHostName = QHostInfo::localHostName(); //获取本地主机名
    qDebug() <<"localHostName: "<<localHostName;
    QHostInfo info = QHostInfo::fromName(localHostName);
    qDebug() <<"IP Address: "<<info.addresses();
    QHostAddress address;
    foreach(address,info.addresses())//获取本地IP地址

    {

         if(address.protocol() == QAbstractSocket::IPv4Protocol)

             serverIpLabel->setText(tr("监听IPv4地址：%1").arg(address.toString()));

         else if(address.protocol() == QAbstractSocket::IPv6Protocol)
     {
          serverIpLabel->setText(tr("监听IPv6地址：%1").arg(address.toString()));
     }
         else if(address.protocol() == QAbstractSocket::UnknownNetworkLayerProtocol)
             return;

    }


    while(!server->listen(address,5500)) {
     QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                        tr("连接测试"),
                                        tr("无法进行连接: %1.")
                                        .arg(server->errorString()),
                                        QMessageBox::Retry
                                        | QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
          {
            close();
            return;
    }
   }

#endif

    serverStatusLabel->setText(tr("服务器监听端口: %1.\n").arg(server->serverPort()));
    setWindowTitle(tr("服务器正在运行中"));
    startButton->setEnabled(false);
}

void TcpDialog::showLog()
{
   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//指定Tr的编码格式
    reqStatusLable->setText(tr("客户端第%1次请求完毕.\n").arg(++count));
}
