#ifndef TCPDIALOG_H
#define TCPDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QLabel;
class QPushButton;
class QTcpSocket;



class TcpServer;

class TcpDialog : public QDialog
{

    Q_OBJECT
public:
    TcpDialog(QWidget *parent = 0);
public slots:
         void showLog();
         void startlisten();

private:
    QLabel *serverStatusLabel;//服务器状态提示
    QLabel *reqStatusLable;  //返回请求次数


    TcpServer *server;      //一个服务器线程对象指针
    QLabel *serverIpLabel;  //本地IP地址显示


    qint32 count;

    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;






};

#endif // TCPDIALOG_H
