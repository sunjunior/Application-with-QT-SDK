#include <QtGui>
#include <QtNetwork>
#include "tcpclient.h"
#include <QTimer>
#include <QSplashScreen>

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),blocksize(0)
{

    clientStatusLabel = new QLabel(tr("客户端就绪"));
    startButton = new QPushButton(tr("开始上传"));
    quitButton = new QPushButton(tr("退出"));



    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));


    connect(&tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));

    connect(&tcpClient,SIGNAL(readyRead()),this,SLOT(getMessage()));

    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));




    QVBoxLayout *mainLayout = new QVBoxLayout;





    mainLayout->addWidget(clientStatusLabel);

    mainLayout->addStretch(5);
    mainLayout->addSpacing(10);

    mainLayout->addStretch(3);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
   // setFixedSize(360,640);
    setWindowTitle(tr("发送数据文件"));



}

void TcpClient::start()
{
    startButton->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    clientStatusLabel->setText(tr("连接中..."));

    tcpClient.connectToHost(QHostAddress("127.0.0.1"),10000);

}













void TcpClient::startTransfer()
{
 //  QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
  // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));//指定QString类的编码格式
    QString name = tr("sunjun");
   // QString name =tr("李四");


    QString pwd = "123456";

  // QString op = "D"; //这个结构体中的各项参数可以由客户端GUI界面获取
   QString op = "A";

   QString sqlstr = "insert into UsersTable (LoginName,Password,RoleID,UserName,SexID,Birthday,  \
           Company,Mobilephone,OfficePhone,Fax,HomePhone,Email,Address,Zipcode) values ('阿桑f','sdfsfsdfsdfdsfd',2,'小张',1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)";
    QString datastr;
    DataWrap *w = new DataWrap();
     datastr = w->sendData(name,pwd,op,sqlstr);//




    qDebug()<< datastr;



    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);

    sendOut<<qint16(0);                          //发送文件头信息大小
    sendOut<<datastr.toUtf8();//按照UTF-8编码发送

    sendOut.device()->seek(0);
    sendOut<<qint16(outBlock.size()-sizeof(qint16));//实际文件的大小

    tcpClient.write(outBlock);

     outBlock.resize(0);




     startButton->setEnabled(true);
     clientStatusLabel->setText(tr("客户端就绪"));


}



void TcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QMessageBox::information(this, tr("网络"),
                             tr("产生如下错误: %1.")
                             .arg(tcpClient.errorString()));

    tcpClient.close();

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
   // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));//指定QString类的编码格式
    clientStatusLabel->setText(tr("客户端准备就绪"));
    startButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}
 /*
void TcpClient::saveLog()
{
    clientProgressBar->setValue(0);
    clientStatusLabel->setText(tr("客户端准备就绪"));


      fileName = QFileDialog::getOpenFileName(this, tr("打开数据库文件"), "F:\\Projects", tr("All Files(*)"));

      QLabel *label1 = new QLabel;
      label1->setText(fileName);
      label1->show();
     QString md5pwd ="ewsfsfweresfdsfa a";
      QString sql = "delete * from infor where age = 22";
      Data *datawrap = (Data*)malloc(sizeof(Data));
      datawrap->operatorCmd='D';
      datawrap->md5Pwd = (char *)malloc(md5pwd.size()+1);
      datawrap->sql = (char *)malloc(md5pwd.size()+1);



      QDateTime now=QDateTime::currentDateTime();




       fileName=now.toString("yyyyMMddhhmmss")+".log"; //在本地建立一个日志文件，然后上传给服务器，由服务器进行处理
       QDir dir(fileName);
       QFile file(fileName);
       file.open(QIODevice::WriteOnly | QIODevice::Truncate);
       QDataStream out(&file);
       out <<op;
       out <<md5pwd;
       out <<sqlstr;
       file.close();

       fileName = dir.absolutePath();
       QLabel *label = new QLabel;
       label->setText(fileName);
       label->show();


      startButton->setEnabled(true);

}

    */

void TcpClient::getMessage()
{
    QByteArray inBlock;

    QDataStream in(&tcpClient);
    in.setVersion(QDataStream::Qt_4_6);
       if (blocksize == 0) {
        if(tcpClient.bytesAvailable() < sizeof(qint16))return;//判断接收的数据是否有两字节，也就是文件的大小信息
        in>>blocksize;
     }

    if(tcpClient.bytesAvailable() < blocksize)//如果数据没有接受全面，返回
       return;
     in>>inBlock;

     blocksize = 0; //传送完了值置为0

     QString str =QString::fromUtf8(inBlock.data());

     QMessageBox::information(this,tr("操作提示"),tr("操作结果:\n%1").arg(str));

}



TcpClient::~TcpClient()
{
    tcpClient.disconnectFromHost();

}
