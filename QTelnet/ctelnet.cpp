#include "ctelnet.h"
#include "qconfigmenu.h"

#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QProcess>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QLineEdit>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QStatusBar>

CTelnet::CTelnet()
{


    deviceIpLabel = new QLabel(tr("设备IP地址:"));
    deviceIpCombox = new QComboBox;
    deviceIpCombox->addItem(tr("192.192.192.198"));
    deviceIpCombox->addItem(tr("191.191.191.178"));
    deviceIpCombox->setCurrentIndex(0);
    linecardIpLabel = new QLabel(tr("线卡IP地址:"));
    deviceRunBtn = new QPushButton(tr("主控连通性测试"));

    linecardIpCombox = new QComboBox;
    linecardIpCombox->addItem(tr("168.2.0.9"));
    linecardIpCombox->addItem(tr("168.2.0.10"));
    linecardIpCombox->addItem(tr("168.2.0.11"));
    linecardIpCombox->addItem(tr("168.2.0.12"));
    linecardIpCombox->addItem(tr("168.2.0.13"));
    linecardIpCombox->addItem(tr("168.2.0.14"));
    linecardIpCombox->addItem(tr("168.2.0.15"));
    linecardIpCombox->addItem(tr("168.2.0.16"));
    linecardIpCombox->setFixedWidth(240);
    linecardIpCombox->setCurrentIndex(6);
    linecardRunBtn = new QPushButton(tr("线卡连通性测试"));
    linecardRunBtn->setFixedWidth(120);

    configBtn = new QPushButton(tr("打开CEP配置界面"));
    configMenu = new QConfigMenu;

    outputLabel =new QLabel(tr("执行结果如下："));
    outputEdit = new QTextEdit;
    outputEdit->setReadOnly(true);


    QGridLayout *glayout = new QGridLayout;
    glayout->setColumnStretch(30,5);
    glayout->addWidget(deviceIpLabel,1,0);
    glayout->addWidget(deviceIpCombox,1,1);
    glayout->addWidget(deviceRunBtn,1,2);
    glayout->addWidget(linecardIpLabel,2,0);
    glayout->addWidget(linecardIpCombox,2,1);
    glayout->addWidget(linecardRunBtn,2,2);
    glayout->addWidget(configBtn,2,3);
    glayout->setSpacing(15);

    statusBar = new QStatusBar(this);
    statusBar->showMessage(tr("Telnet自动化配置工具 Made by SUNJUN. CopyLeft@ZTE Corp. All rights reserved"));


    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->addLayout(glayout);
    vlayout->addWidget(outputLabel);
    vlayout->addWidget(outputEdit);
    vlayout->addWidget(statusBar);
    setLayout(vlayout);



    setFixedSize(640,480);
    setWindowTitle(tr("Telnet连通性测试工具"));
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);


    QIcon icon = QIcon(":/images/app.png");
    iTrayIcon = new QSystemTrayIcon;
    iTrayIcon->setIcon(icon);
    iTrayIcon->setVisible(true);

    iTrayIcon->showMessage(tr("提示"), tr("本程序每15s添加登录线卡路由信息一次"));
    minimizeAction = new QAction(tr("&最小化"), this);
    connect(minimizeAction,SIGNAL(triggered()), configMenu, SLOT(hide()));//先最小化子窗体
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(tr("&显示主界面"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(tr("&退出"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    iTraymenu=new QMenu();
    iTraymenu->addAction(minimizeAction);
    iTraymenu->addAction(restoreAction);
    iTraymenu->addAction(quitAction);
    iTraymenu->addSeparator();
    iTrayIcon->setContextMenu(iTraymenu);
    connect(iTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    cmd = new QProcess;

    connect(linecardRunBtn, SIGNAL(clicked(bool)), this, SLOT(linecardRunClicked()));
    connect(deviceRunBtn, SIGNAL(clicked(bool)), this, SLOT(deviceRunClicked()));
    connect(configBtn,SIGNAL(clicked()),this, SLOT(showConfigMenu()));
    connect(cmd, SIGNAL(readyRead()), this, SLOT(readOutput()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerDone()));
    connect(timer,SIGNAL(timeout()), this, SLOT(getAllIp()));
    timer->start(15000);
    QTimer::singleShot(5000, this, SLOT(timerDone()));//程序起来后先等待5s，然后再执行添加线卡路由信息一次
    time = new QTime;
    time->start(); //开始计时，以ms为单位

}


CTelnet::~CTelnet()
{

}

void CTelnet::showConfigMenu(void)
{
    configMenu->setWindowModality(Qt::ApplicationModal);
    configMenu->show();

}


void CTelnet::getAllIp()
{
    QString addrList = "";
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress hostAddress, list)
    {
        if( "127.0.0.1" != hostAddress.toString())//屏蔽掉本地环回地址
        {
            addrList += hostAddress.toString();
            addrList += " ";
        }
    }
    iTrayIcon->setToolTip(addrList);
}

void CTelnet::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //触发后台图标执行相应事件
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}


void CTelnet::timerDone()
{

    QString input =  "route delete " + linecardIpCombox->currentText();
    QProcess::execute(input);
    input = "route add " + linecardIpCombox->currentText() + " mask 255.255.255.255 " + deviceIpCombox->currentText();
    QProcess::execute(input);

}

void CTelnet::getRunTime()
{
    int time_Diff = time->elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    //以下方法是将ms转为s
    int total_seconds = time_Diff / 1000;
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600)/60;
    int seconds = total_seconds % 60;
    iTrayIcon->showMessage(tr("提示"),tr("本程序总运行时长为%1小时%2分钟%3秒").arg(hours).arg(minutes).arg(seconds));

}

void CTelnet::closeEvent(QCloseEvent *event)
{
//    switch( QMessageBox::information( this, tr("关闭提示"),
//                                     tr("确认退出程序？按确定退出程序；按取消最小化至系统托盘"),tr("确定"), tr("取消"),0, 1))
//    {
//    case 0:
//        event->accept();
//        break;
//    case 1:
//        event->ignore();
//        this->hide();
//        iTrayIcon->showMessage(tr("提示"),tr("本程序每15s添加登录线卡路由信息一次"));
//        break;
//    default:
//        break;
//    }

    event->ignore();
    this->hide();
    getRunTime();

}

void CTelnet::deviceRunClicked()
{
    output = "";

    QString input =  "ping -n 2 " + deviceIpCombox->currentText();

    if(cmd->state() == QProcess::Running)
    {
        QMessageBox::critical(this,tr("错误"),tr("已有一个实例正在运行中，请稍后再试！"),tr("确认"));
        return;
    }

    cmd->start(input);


    outputEdit->setText(output);

}

void CTelnet::linecardRunClicked()
{

    output = "";

    QString input =  "ping -n 2 " + linecardIpCombox->currentText();

    if(cmd->state() == QProcess::Running)
    {
        QMessageBox::critical(this,tr("错误"),tr("已有一个实例正在运行中，请稍后再试！"),tr("确认"));
        return;
    }

    cmd->start(input);

    outputEdit->setText(output);


}

void CTelnet::readOutput()
{

    output += cmd->readAll();
    outputEdit->setText(output);

}
