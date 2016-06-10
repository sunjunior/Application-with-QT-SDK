#ifndef TCPCLIENT_H
#define TCPCLIENT_H


#include "datawrap.h"
#include <QWidget>
#include <QTcpSocket>
#include <QDateTime>


class QDialogButtonBox;
class QLabel;
class QProgressBar;
class QPushButton;
class QTcpSocket;


class TcpClient : public QWidget
{
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = 0);
            ~TcpClient();
signals:

public slots:
    void start();
    void startTransfer();
    void displayError(QAbstractSocket::SocketError socketError);
    void getMessage();



private:

    QLabel *clientStatusLabel;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *openButton;
    QDialogButtonBox *buttonBox;
    QTcpSocket tcpClient;
    QByteArray outBlock; //


    qint16 blocksize;




};

#endif // TCPCLIENT_H
