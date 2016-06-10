#include "qclient.h"
#include <QtGui>
#include <QHostAddress>
#include <QNetworkInterface>



QClient::QClient(QWidget *parent) :
    QWidget(parent)
{
    m_statusLabel = new QLabel(tr("服务端返回结果："));
    m_showMsgEdit = new QTextEdit(this);
    m_showMsgEdit->setReadOnly(true);
    m_showMsgEdit->setTextColor(Qt::green);

    m_sendMsgEdit = new QTextEdit(this);
    m_sendMsgEdit->setTextColor(Qt::blue);
    m_sendMsgEdit->installEventFilter(this);
    m_sendMsgEdit->setToolTip(tr("按Ctrl+Enter快捷键可以发送消息"));
    m_ServerLabel = new QLabel(tr("服务器IP："));
    QRegExp regExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
    m_ServerIp = new QLineEdit(tr("192.192.192.198"));
    m_ServerIp->setValidator(new QRegExpValidator(regExp,this));

    m_PortNo = new QLabel(tr("端口:"));
    m_PortEdit = new QLineEdit(tr("23"));
    m_PortEdit->setValidator(new QIntValidator(23,65535, this));
    m_connectBtn = new QPushButton(tr("连接服务器"));
    m_disconnectBtn = new QPushButton(tr("断开连接"));
    m_connectBtn->setEnabled(true);
    m_disconnectBtn->setEnabled(false);
    m_sendMsgBtn = new QPushButton(tr("发送消息"));
    m_configBtn = new QPushButton(tr("加载默认配置"));
    m_showLogBtn = new QPushButton(tr("保存日志文件"));
    QHBoxLayout *hlayout = new QHBoxLayout;

    hlayout->addWidget(m_ServerLabel);
    hlayout->addWidget(m_ServerIp);
    hlayout->addWidget(m_PortNo);
    hlayout->addWidget(m_PortEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(m_connectBtn, QDialogButtonBox::ActionRole);
    buttonBox->addButton(m_disconnectBtn,QDialogButtonBox::ActionRole);
    buttonBox->addButton(m_sendMsgBtn, QDialogButtonBox::RejectRole);
    buttonBox->addButton(m_configBtn, QDialogButtonBox::RejectRole);
    buttonBox->addButton(m_showLogBtn, QDialogButtonBox::RejectRole);
    m_connectBtn->setEnabled(true);
    m_disconnectBtn->setEnabled(false);

    m_statusBar = new QStatusBar(this);
    m_statusBar->showMessage(tr("客户端准备就绪"));
    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(m_statusLabel);
    vLayout->addStretch(5);
    vLayout->addSpacing(10);
    vLayout->addWidget(m_showMsgEdit);
    vLayout->addStretch(3);
    vLayout->addSpacing(5);
    vLayout->addWidget(m_sendMsgEdit);
    vLayout->addLayout(hlayout);
    vLayout->addWidget(buttonBox,1, Qt::AlignHCenter);
    vLayout->addStretch(3);
    vLayout->addSpacing(5);
    vLayout->addWidget(m_statusBar, 0, Qt::AlignBottom);
    setLayout(vLayout);
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    setWindowTitle(tr("Telnet测试客户端"));
    setWindowIcon(QIcon(":/images/telnet.png"));
    setFixedSize(640, 480);

    connect(m_connectBtn, SIGNAL(clicked()), this, SLOT(slotstartConnection()));
    connect(m_disconnectBtn, SIGNAL(clicked()), this, SLOT(slotcloseConnection()));
    connect(m_sendMsgBtn, SIGNAL(clicked()), this, SLOT(slotsendMessage()));
    connect(m_showMsgEdit, SIGNAL(textChanged()), this, SLOT(slotautoScroll()));
    connect(m_configBtn, SIGNAL(clicked()), this, SLOT(slotautoConfig()));
    connect(m_showLogBtn, SIGNAL(clicked()), this, SLOT(slotshowLog()));
    m_runConfigEdit = new QTextEdit;
    m_pSocket = new QTcpSocket(this);

}


QClient::~QClient()
{
    if (m_pSocket)
    {
         m_pSocket->disconnectFromHost();
    }

}

void QClient::slotstartConnection()
{
    if (m_ServerIp->text().isEmpty() || m_PortEdit->text().isEmpty())
    {
        QMessageBox::critical(this,tr("错误"),tr("服务器IP地址或端口为空，请确认输入后再试！"),tr("确认"));
        return;
    }

    m_connectBtn->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_statusBar->showMessage(tr("连接中..."));
    m_pSocket->abort();

    m_pSocket->connectToHost(QHostAddress(m_ServerIp->text()), m_PortEdit->text().toInt());

    connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(slotdisplayError(QAbstractSocket::SocketError)));
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(slotreadMessage()));
    connect(m_pSocket, SIGNAL(connected()), this, SLOT(slotshowStatus()));
    connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(slotshowStatus()));


    m_showMsgEdit->clear();//清空输出结果


}

bool QClient::eventFilter(QObject *obj, QEvent *e)
{
    Q_ASSERT(obj == m_sendMsgEdit);
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(e);
        if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
        {
            slotsendMessage(); //发送消息的槽
            return true;
        }
    }

    return false;
}

void QClient::slotcloseConnection()
{
    if(m_pSocket->state() == QAbstractSocket::ConnectedState)
    {
        m_pSocket->close();
    }

    output = "";

}

void QClient::slotautoScroll()
{
    m_showMsgEdit->moveCursor(QTextCursor::End);
}

void QClient::slotshowLog()
{
    QFile logfile("output.log");
    if(!logfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("错误提示"), tr("无法生成日志文件"), tr("确定"));
        return;
    }
    QTextStream out(&logfile);
    out << m_showMsgEdit->toPlainText();
    logfile.close();
    QMessageBox::information(this,tr("提示"),tr("成功生成输出日志文件！"),tr("确认"));

}

void QClient::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();

}

void QClient::slotshowStatus()
{
    switch(m_pSocket->state())
    {
    case QAbstractSocket::UnconnectedState:
    {
        m_statusBar->showMessage(tr("已断开连接！"));
        m_connectBtn->setEnabled(true);
        m_disconnectBtn->setEnabled(false);
        break;
    }

    case QAbstractSocket::ConnectingState:
    {
        m_statusBar->showMessage(tr("正在连接状态！"));
        break;
    }

    case QAbstractSocket::ConnectedState:
    {
        m_statusBar->showMessage(tr("连接已建立！"));
        m_connectBtn->setEnabled(false);
        m_disconnectBtn->setEnabled(true);
        break;
    }

    default:
        break;

    }
    QApplication::restoreOverrideCursor();
    output = "";

}


void QClient::slotreadMessage()
{
    output += m_pSocket->readAll();

    m_showMsgEdit->setText(output);


}

void QClient::slotsendMessage()
{
    if (m_sendMsgEdit->toPlainText().isEmpty())
    {
        QMessageBox::critical(this,tr("错误"),tr("发送内容不能为空！"),tr("确认"));
        return;
    }
    m_sendMsgBtn->setEnabled(false);
    QString str = m_sendMsgEdit->toPlainText();

    QStringList cmdList = str.split("\n");
    foreach(QString cmd, cmdList)
    {
        cmd += "\n";
        m_pSocket->write(cmd.toAscii());

       delaymsec(1000);

    }
    m_sendMsgBtn->setEnabled(true);
}

void QClient::delaymsec(int msec)
{

    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }


}

void QClient::slotautoConfig()
{

    QFile configfile("config.txt");
    if(!configfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("打开提示"), tr("配置文件config.txt无法打开,请确认此文件是否生成！"), tr("确定"));
        return;
    }
    QTextStream in(&configfile);
    m_sendMsgEdit->setText(in.readAll());
    configfile.close();

}

void QClient::slotdisplayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
           return;

       QMessageBox::information(this, tr("网络提示"),
                                tr("产生如下错误: %1.")
                                .arg(m_pSocket->errorString()));

       m_pSocket->close();

       m_statusBar->showMessage(tr("客户端准备就绪"));

       m_connectBtn->setEnabled(true);
       m_disconnectBtn->setEnabled(false);
       QApplication::restoreOverrideCursor();

}

