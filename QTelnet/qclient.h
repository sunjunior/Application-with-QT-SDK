#ifndef QCLIENT_H
#define QCLIENT_H

#include <QtNetwork/QTcpSocket>
#include <QWidget>

class QPushButton;
class QTextEdit;
class QLabel;
class QLineEdit;
class QStatusBar;


class QClient : public QWidget
{
    Q_OBJECT
public:
    explicit QClient(QWidget *parent = 0);
             ~QClient();

private slots:
    void slotreadMessage();
    void slotsendMessage();
    void slotstartConnection();
    void slotcloseConnection();
    void slotdisplayError(QAbstractSocket::SocketError socketError);
    void slotshowStatus();
    void slotautoScroll();
    void slotshowLog();
    void slotautoConfig();

private:
    void closeEvent(QCloseEvent *event);
    void delaymsec(int msec);
    bool eventFilter(QObject *obj, QEvent *e);

private:
    QLabel *m_statusLabel;
    QTextEdit *m_showMsgEdit;
    QTextEdit *m_sendMsgEdit;
    QPushButton *m_connectBtn;
    QPushButton *m_disconnectBtn;
    QPushButton *m_sendMsgBtn;
    QTcpSocket *m_pSocket;
    QLabel *m_ServerLabel;
    QLineEdit *m_ServerIp;
    QLabel *m_PortNo;
    QLineEdit *m_PortEdit;
    QStatusBar *m_statusBar;
    QPushButton *m_showLogBtn;
    QPushButton *m_configBtn;

    QString output;
    QTextEdit *m_runConfigEdit;


};

#endif // QCLIENT_H

