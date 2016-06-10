#include "qwincomm.h"
#include "win_qextserialport.h"
#include <QtGui>

QWinComm::QWinComm(QWidget *parent) :
    QWidget(parent)
{
    m_showMsgLbl = new QLabel(tr("返回数据:"));
    m_showTextEdit = new QTextEdit;
    m_sendLineEdit = new QLineEdit;
    m_sendMsgBtn = new QPushButton(tr("发送消息"));

    QHBoxLayout *hlayout  = new QHBoxLayout;
    hlayout->addWidget(m_sendLineEdit);
    hlayout->addWidget(m_sendMsgBtn);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(m_showMsgLbl);
    vlayout->addWidget(m_showTextEdit);
    vlayout->addLayout(hlayout);

    setLayout(vlayout);

    setWindowTitle(tr("Comm串口通信工具"));

    pComm = new Win_QextSerialPort("COM1", QextSerialBase::EventDriven);
    pComm->open(QIODevice::ReadWrite);
    pComm->setBaudRate(BAUD115200);
    pComm->setDataBits(DATA_8);
    pComm->setParity(PAR_NONE);
    pComm->setStopBits(STOP_1);
    pComm->setFlowControl(FLOW_OFF);
    pComm->setTimeout(300);

    connect(pComm, SIGNAL(readyRead()), this, SLOT(slotreadMessage()));
    connect(m_sendMsgBtn, SIGNAL(clicked()), this, SLOT(slotsendMessage()));


}

void QWinComm::slotreadMessage()
{
    qDebug() << pComm->bytesAvailable() << endl;
    if (pComm->bytesAvailable() <= 8)
    {
        output = pComm->readAll();
        qDebug() << output << endl;
        //m_showTextEdit->insertPlainText(output);
    }


}

void QWinComm::slotsendMessage()
{
    pComm->write(m_sendLineEdit->text().toAscii());
}
