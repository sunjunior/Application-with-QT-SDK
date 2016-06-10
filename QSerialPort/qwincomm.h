#ifndef QWINCOMM_H
#define QWINCOMM_H

#include <QWidget>

class Win_QextSerialPort;
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;

class QWinComm : public QWidget
{
    Q_OBJECT
public:
    explicit QWinComm(QWidget *parent = 0);

private slots:
    void slotsendMessage(void);
    void slotreadMessage(void);


private:
    QLabel *m_showMsgLbl;
    QTextEdit *m_showTextEdit;
    QLineEdit *m_sendLineEdit;
    QPushButton *m_sendMsgBtn;
    Win_QextSerialPort *pComm;
    QString output;
};

#endif // QWINCOMM_H
