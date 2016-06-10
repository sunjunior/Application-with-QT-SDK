#include "tcpthread.h"
#include "operatorsql.h"
#include <QMessageBox>

#include <QDateTime>
#include <QTextCodec>

#include <QFile>
#include <QMutex>
#include <QSqlTableModel>




TcpThread::TcpThread(int socketDescriptor,QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
 //  this->start();

  blocksize = 0;

}

void TcpThread::run(){

    tcpSocket=new QTcpSocket; //服务器连接上后与客户端通讯的子套接字
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket->error());
        return;
    }

     connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(getClientData()),Qt::DirectConnection);



 //   connect(time,SIGNAL(timeout()),this,SLOT(judge()),Qt::DirectConnection);//



    tcpSocket->waitForDisconnected();

      //   QTimer::singleShot(2000, this, SLOT(judge()));
        // connect(timer, SIGNAL(timeout()), this, SLOT(judge()),Qt::DirectConnection);


     //   exec();


}




 void TcpThread::getClientData()
{


    QDataStream in(tcpSocket);

    in.setVersion(QDataStream::Qt_4_6);
       if (blocksize == 0) {
        if(tcpSocket->bytesAvailable() < sizeof(qint16))return;//判断接收的数据是否有两字节，也就是文件的大小信息
        in>>blocksize;
     }

    if(tcpSocket->bytesAvailable() < blocksize)//如果数据没有接受全面，返回
       return;
     in>>inBlock;

     blocksize =0; //传送完了值置0

     QString str =QString::fromUtf8(inBlock.data());

        list = str.split("\n"); //分割发送方的字符串

        for(int i =0; i< list.size();i++)
          data[i] = list.at(i);


      opeatorData();    //操作数据库


  tcpSocket->disconnectFromHost();


}

 void TcpThread::opeatorData()
 {
      OperatorSql w;
      char ch = data[2].at(0).toAscii(); //将QChar 转换为标准C语言的ASCII，上网找了，switch在QT中只能支持整型对象
                      //操作数据库

      if (!w.createConnection()) {


         sendMsg(tr("SQL_LINK_ERROR"));

         return;
      }


/*
      if(!w.verifyUser(data[0],data[1]))//用户名密码错误
     {


         sendMsg(tr("USER_PASSWORD_ERROR"));
         tcpSocket->disconnectFromHost();//断开连接
      QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
         qDebug()<< tr("用户或者密码不正确！")<<"\n";

         return;
     }

*/

       QString sqlstr = data[3];//取出SQL语句
         switch(ch)
         {

             case 'Q':  {

            w.queryRecode(sqlstr);

             saveLog();
             break;//保存日志
         }
         case 'A':  {
            if(w.addRecode(sqlstr))
            {
            sendMsg(tr("ADD_SUCCESS"));
            saveLog();
             break;
            }
            else
             {
                 sendMsg(tr("ADD_FAILED"));
                 break;
             }
             }
         case 'D': {

            if(w.deleteRecode(sqlstr))
            {
             sendMsg(tr("DELETE_SUCCESS")) ;
             saveLog();
             break;
         }
            else
             {
                 sendMsg(tr("DELETE_FAILED"));
                 break;
             }
              }
         case 'U':   {
            if( w.updateRecode(sqlstr)) {
              sendMsg(tr("UPDATE_SUCCESS"));
             saveLog();
             break;
         }
         else
                 {
                     sendMsg(tr("UPDATE_FAILED"));
                     break;
                 }
            }
             default:  sendMsg(tr("UNKOWN_ERROR")); break;


        }

}

 void TcpThread::saveLog()        //保存客户端操作的日志文件
 {

     QFile logFile("./Log/log.txt"); //加入日志文件
         if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
             return;
     QDateTime time = QDateTime::currentDateTime();
     QString now = time.toString("yyyy-MM-dd hh:mm:ss");


         QTextStream out(&logFile);

   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//指定Tr的编码格式
        out.setCodec(QTextCodec::codecForName("UTF-8"));
         out<<data[0]<<tr("于")<<now<<tr("进行了数据库操作：")<<data[3]<<"\n";
         logFile.close();

         qDebug()<<tr(" %1于 %2 进行了数据库操作:%3.").arg(data[0]).arg(now).arg(data[3])<<"\n";

 }

 void TcpThread::sendMsg(const QString message)

 {


     QByteArray outBlock;
    QDataStream out(&outBlock,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out<<qint16(0);                          //发送文件头信息大小
    out<<message.toUtf8();//按照UTF-8编码发送

    out.device()->seek(0);
    out<<qint16(outBlock.size()-sizeof(qint16));//实际文件的大小

    tcpSocket->write(outBlock);

     outBlock.resize(0);




 }






TcpThread::~TcpThread()
{


     delete tcpSocket;

}

