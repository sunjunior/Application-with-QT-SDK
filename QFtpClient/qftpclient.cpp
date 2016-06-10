#include "qftpclient.h"
#include <QtGui>
#include <QtNetwork>


QFtpClient::QFtpClient(QWidget *parent)
    : QWidget(parent),ftp(0)
{

    ftpServerLabel = new QLabel(tr("FTP服务器地址："));
    QRegExp regExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
    ftpServerLineEdit = new QLineEdit("10.10.210.155");
    ftpServerLineEdit->setValidator(new QRegExpValidator(regExp,this));

    ftpPortLabel = new QLabel(tr("FTP端口号:"));
    ftpPortLineEdit = new QLineEdit(tr("21"));
    ftpPortLineEdit->setFixedWidth(50);
    ftpPortLineEdit->setValidator(new QIntValidator(21,65535,this));

    timerIntervalLabel = new QLabel(tr("取版本周期:"));
    timerIntervalSpinBox = new QSpinBox;
    timerIntervalSpinBox->setValue(24);
    timerIntervalSpinBox->setRange(1,24);
    timerIntervalSpinBox->setFixedWidth(80);
    timerIntervalSpinBox->setToolTip(tr("取版本间隔周期(小时),即程序每隔多少小时执行一次取版本任务"));
    connect(timerIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotupdateDateTimer()));

    connectButton = new QPushButton(tr("已经断开"));
    connectButton->setDefault(true);
    connectButton->setEnabled(false);
    connectButton->setFont(QFont("msyh",10,QFont::Bold,FALSE));


    loginnameLabel = new QLabel(tr("FTP登录用户名:"));
    loginnameLineEdit = new QLineEdit(tr("switch"));

    passwordLabel = new QLabel(tr("FTP密码:"));
    passwordLineEdit = new QLineEdit(tr("system_221"));
    passwordLineEdit->setEchoMode(QLineEdit::Password);



    calendar = new QCalendarWidget;
    calendar->setWindowFlags(windowFlags() & ~(Qt::WindowMinMaxButtonsHint));
    calendar->setWindowTitle(tr("选择日期"));
    calendar->setWindowIcon(QIcon(":/images/time.png"));
    calendar->setMinimumDate(QDate(1970, 1, 1));
    calendar->setMaximumDate(QDate(2100, 1, 1));
    calendar->setGridVisible(true);

    isDownloadCheckBox = new QCheckBox(tr("下载ZAR文件"));
    isDownloadCheckBox->setChecked(true);
    isRebootCheckBox = new QCheckBox(tr("重启设备"));
    isRebootCheckBox->setChecked(true);

    currentDateLabel = new QLabel(tr("版本日期:"));
    currentDateEdit = new QLineEdit;
    currentDateEdit->setReadOnly(true);
    currentDateEdit->setText(calendar->selectedDate().toString("yyyy-MM-dd"));


    connect(calendar, SIGNAL(selectionChanged()),
            this, SLOT(selectedDateChanged()));

    changeDateBtn = new QPushButton(tr("更改日期"));
    connect(changeDateBtn, SIGNAL(clicked()), this, SLOT(getZarFileDate()));
    connect(this,  SIGNAL(dateChanged(const QDate &)), calendar, SLOT(setSelectedDate(QDate)));

    zarPathLabel = new QLabel(tr("ZAR文件存放目录:"));
    zarPathLabel->setFixedWidth(120);
    zarpathEdit = new QLineEdit;
    zarpathEdit->setReadOnly(true);
    zarpathEdit->setText(QCoreApplication::applicationDirPath());
    zarpathBtn = new QPushButton(tr("更改路径"));

    scriptPathLabel = new QLabel(tr("配置脚本存放目录:"));
    scriptPathLabel->setFixedWidth(120);
    scriptpathEdit = new QLineEdit;
    scriptpathEdit->setReadOnly(true);
    scriptpathEdit->setText(QCoreApplication::applicationDirPath());
    scriptpathBtn = new QPushButton(tr("更改路径"));

    m_filesLabel = new QLabel(tr("当前执行脚本文件:"));
    m_filesLabel->setFixedWidth(120);
    m_filesBox   = new QComboBox;
    m_filesBox->setToolTip(tr("设备配置脚本文件名需包括设备IP地址，配置文件是按照设备对应的脚本名称查找的!"));

    deviceIpLabel = new QLabel(tr("Telnet设备IP地址:"));
    deviceIpCombox = new QComboBox;
    deviceIpCombox->setFixedWidth(200);
    deviceIpAddBtn = new QPushButton(tr("增加设备IP"));

    telnetPortLabel = new QLabel(tr("Telnet端口:"));
    telnetPortLineEdit = new QLineEdit(tr("23"));
    telnetPortLineEdit->setValidator(new QIntValidator(23,65535,this));

    deviceUsernameLabel = new QLabel(tr("Telnet登录用户名:"));
    deviceUsernameLineEdit = new QLineEdit(tr("who"));
    devicePasswordLabel = new QLabel(tr("Telnet密码:"));
    devicePasswordLineEdit = new QLineEdit(tr("who"));
    devicePasswordLineEdit->setEchoMode(QLineEdit::Password);


    fileList = new QTreeWidget;
    fileList->setEnabled(false);
    fileList->setRootIsDecorated(false);
    fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileList->setHeaderLabels(QStringList() << tr("文件名") << tr("大小(MB)") << tr("拥有者") << tr("所属组") << tr("修改时间"));
    fileList->header()->setStretchLastSection(false);
    fileList->header()->setResizeMode(QHeaderView::ResizeToContents);

    statusBar = new QStatusBar;
    statusBar->showMessage("每日构建自动配置客户端. 工具制作人员SunJun@ZTE Corp. All rights reserved.");

    progressDialog = new QProgressDialog(this);

    progressDialog->setWindowIcon(QIcon(":/images/progressbar.png"));
    progressDialog->setCancelButton(new QPushButton(tr("取消下载")));
    progressDialog->setWindowTitle(tr("ZAR文件下载"));
    progressDialog->setWindowFlags(windowFlags() &~ (Qt::WindowContextHelpButtonHint |Qt::WindowMinMaxButtonsHint| Qt::WindowSystemMenuHint));

    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));


    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(ftpServerLabel);
    hlayout1->addWidget(ftpServerLineEdit);
    hlayout1->addWidget(ftpPortLabel);
    hlayout1->addWidget(ftpPortLineEdit);
    hlayout1->addSpacerItem(new QSpacerItem(10,5));
    hlayout1->addWidget(timerIntervalLabel);
    hlayout1->addWidget(timerIntervalSpinBox);
    hlayout1->addWidget(connectButton);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(loginnameLabel);
    hlayout2->addWidget(loginnameLineEdit);
    hlayout2->addWidget(passwordLabel);
    hlayout2->addWidget(passwordLineEdit);

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addWidget(isDownloadCheckBox);
    hlayout3->addWidget(isRebootCheckBox);
    hlayout3->addSpacing(10);
    hlayout3->addStretch(5);
    hlayout3->addWidget(currentDateLabel);
    hlayout3->addWidget(currentDateEdit);
    hlayout3->addWidget(changeDateBtn);

    QHBoxLayout *hlayout4 = new QHBoxLayout;
    hlayout4->addWidget(zarPathLabel);
    hlayout4->addWidget(zarpathEdit);
    hlayout4->addWidget(zarpathBtn);

    QHBoxLayout *hlayout5 = new QHBoxLayout;
    hlayout5->addWidget(scriptPathLabel);
    hlayout5->addWidget(scriptpathEdit);
    hlayout5->addWidget(scriptpathBtn);

    QHBoxLayout *hlayout6  = new QHBoxLayout;
    hlayout6->addWidget(m_filesLabel);
    hlayout6->addWidget(m_filesBox);


    QHBoxLayout *hlayout7 = new QHBoxLayout;
    hlayout7->addWidget(deviceIpLabel);
    hlayout7->addWidget(deviceIpCombox);
    hlayout7->addWidget(deviceIpAddBtn);
    hlayout7->addSpacing(5);
    hlayout7->addStretch(3);
    hlayout7->addWidget(telnetPortLabel);
    hlayout7->addWidget(telnetPortLineEdit);

    QHBoxLayout *hlayout8 = new QHBoxLayout;
    hlayout8->addWidget(deviceUsernameLabel);
    hlayout8->addWidget(deviceUsernameLineEdit);
    hlayout8->addWidget(devicePasswordLabel);
    hlayout8->addWidget(devicePasswordLineEdit);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout4);
    vlayout->addLayout(hlayout5);
    vlayout->addLayout(hlayout6);
    vlayout->addLayout(hlayout7);
    vlayout->addLayout(hlayout8);
    vlayout->addWidget(fileList);
    vlayout->addWidget(statusBar);
    setLayout(vlayout);

    setWindowTitle(tr("6300V2.1每日构建自动配置客户端"));
    setWindowIcon(QIcon(":/images/app.png"));


    iTrayIcon = new QSystemTrayIcon;
    iTrayIcon->setIcon(QIcon(":/images/traydark.png"));
    iTrayIcon->setVisible(true);

    minimizeAction = new QAction(tr("最小化"), this);
    minimizeAction->setIcon(QIcon(":/images/minimize.png"));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(showMinimized()));

    maximumAction = new QAction(tr("最大化"), this);
    maximumAction->setIcon(QIcon(":/images/maximum.png"));
    connect(maximumAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    freetimeAction = new QAction(tr("查看运行时长"), this);
    freetimeAction->setIcon(QIcon(":/images/time.png"));
    connect(freetimeAction, SIGNAL(triggered()), this, SLOT(getRunTime()));

    restartAction = new QAction(tr("重启程序"), this);
    restartAction->setIcon(QIcon(":/images/restart.png"));
    connect(restartAction, SIGNAL(triggered()), this, SLOT(slotRestart()));

    openZarDirAction = new QAction(tr("打开ZAR目录"), this);
    openZarDirAction->setIcon(QIcon(":/images/opendir.png"));
    connect(openZarDirAction, SIGNAL(triggered()), this, SLOT(slotOpenZarDir()));

    restoreAction = new QAction(tr("显示主界面"), this);
    restoreAction->setIcon(QIcon(":/images/menu.png"));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    contactAction = new QAction(tr("关于作者"), this);
    contactAction->setIcon(QIcon(":/images/contact.png"));
    connect(contactAction, SIGNAL(triggered()), this, SLOT(slotContactUs()));

    quitAction = new QAction(tr("退出"), this);
    quitAction->setIcon(QIcon(":/images/close.png"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    iTraymenu=new QMenu();
    iTraymenu->addAction(minimizeAction);
    iTraymenu->addAction(maximumAction);
    iTraymenu->addAction(freetimeAction);
    iTraymenu->addAction(restartAction);
    iTraymenu->addAction(openZarDirAction);
    iTraymenu->addAction(restoreAction);
    iTraymenu->addAction(contactAction);
    iTraymenu->addAction(quitAction);
    iTraymenu->addSeparator();
    iTrayIcon->setContextMenu(iTraymenu);
    connect(iTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    savedFilePath = "/";

    loginTimer = new QTimer;
    connect(loginTimer, SIGNAL(timeout()), this, SLOT(timeDone()));//登录超时


    m_zarTimer = new QTimer;//每10s轮询一次，检测是否存在当日每日构建版本；同时获取本机所有IP地址
    m_zarTimer->setInterval(10000);
    connect(m_zarTimer, SIGNAL(timeout()), this, SLOT(slotAutoDownload()));
    connect(m_zarTimer, SIGNAL(timeout()), this, SLOT(getAllIp()));//显示所有IP地址


    m_dateTimer = new QTimer;//每隔24小时轮询一次
    m_dateTimer->setInterval(timerIntervalSpinBox->value()*3600000);
    connect(m_dateTimer, SIGNAL(timeout()), this, SLOT(timerinit()));

    m_calendarTimer = new QTimer; //每隔24小时更新一次
    m_calendarTimer->setInterval(timerIntervalSpinBox->value()*3600000);
    connect(m_calendarTimer, SIGNAL(timeout()), this, SLOT(slotUpdateDate()));

    m_updateTimer = new QTimer;//每30s更新ftp文件列表
    m_updateTimer->setInterval(30000);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateList()));



    connect(this, SIGNAL(done()), this, SLOT(slotTelnetDevice())); //zar文件下载完成后，开始执行连通性测试程序

    connect(zarpathBtn, SIGNAL(clicked()), this, SLOT(changeFilePath())); //修改zar文件存放目录
    connect(scriptpathBtn, SIGNAL(clicked()), this, SLOT(changeScriptPath())); //修改脚本配置文件存放目录

    connect(deviceIpAddBtn, SIGNAL(clicked()), this, SLOT(slotDeviceIpAdd()));
    QTimer::singleShot(60000, this, SLOT(timerinit()));//程序初始化时，等待1分钟，便于修改配置文件, 启动定时下载器

    m_pSocket = new QTcpSocket; //初始化socket
    telnetTimer = new QTimer;//Telnet登录定时器

    isIconDark = false;
    iconTimer = new QTimer;
    iconTimer->setInterval(1000);
    connect(iconTimer, SIGNAL(timeout()), this, SLOT(slotAutoIconChange()));

    time = new QTime; //初始化取版本周期内程序休眠时间
    time->start();

    iTrayIcon->showMessage(tr("提示"), tr("程序启动后将有1分钟时间来配置，请在此时间内修改相关参数！"));

}

void QFtpClient::slotOpenZarDir()
{
    QString path = zarpathEdit->text();//获取ZAR文件存放目录
    QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));

}

void QFtpClient::slotAutoIconChange()
{
    if (isIconDark)
    {
        iTrayIcon->setIcon(QIcon(":/images/traydark.png"));
        isIconDark = false;

    }
    else
    {
        iTrayIcon->setIcon(QIcon(":/images/tray.png"));
        isIconDark = true;

    }

}

void QFtpClient::slotupdateDateTimer()
{
    if (!m_dateTimer->isActive() && !m_calendarTimer->isActive())//定时器激活时才重新启用定时器
    {
       return;
    }
    m_dateTimer->stop();//修改周期时间值有变化时，重新启用定时器
    m_calendarTimer->stop();
    m_dateTimer->setInterval(timerIntervalSpinBox->value()*3600000);
    m_calendarTimer->setInterval(timerIntervalSpinBox->value()*3600000);
    m_dateTimer->start();
    m_calendarTimer->start();
    time->restart();
    iTrayIcon->showMessage(tr("提示"), tr("更新取版本周期时间成功，程序将间隔%1小时后重新取版本!")
                           .arg(timerIntervalSpinBox->value()));

}

void QFtpClient::slotDeviceIpAdd(void)
{
    bool ok = false;
         QString text = QInputDialog::getText(this, tr("增加Telnet设备"),
                                              tr("设备IP地址:"), QLineEdit::Normal,
                                              tr("192.192.192.198"), &ok);
         if (ok && !text.isEmpty())
         {
             if (deviceIpCombox->count() == 0)
             {
                 deviceIpCombox->addItem(text);
             }
             else
             {
                 for(int fileindex = 0; fileindex < deviceIpCombox->count(); fileindex++)
                 {
                     if (deviceIpCombox->itemText(fileindex) == text)
                     {
                         QMessageBox::information(this,tr("提示"),tr("输入设备IP地址已经存在！"),tr("确认"));
                         return;
                     }
                 }
                 deviceIpCombox->addItem(text);

             }
         }
}

void QFtpClient::slotUpdateDate()
{
    dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");//刷新每日构建版本日期
    currentDateEdit->setText(dateTime);
    QString log = tr("程序自动更新版本日期为:%1.").arg(dateTime);
    iTrayIcon->showMessage(tr("提示"), log);
    recordLog(log);
    QDate date = QDate::fromString(dateTime, "yyyy-MM-dd");
    emit dateChanged(date);
}

void QFtpClient::selectedDateChanged()
{
    currentDateEdit->setText(calendar->selectedDate().toString("yyyy-MM-dd"));

}

void QFtpClient::getZarFileDate(void)
{
    calendar->setWindowModality(Qt::ApplicationModal);
    calendar->show();
}

void QFtpClient::slotContactUs(void)
{
    const QString OnlineHelpURL("http://blog.csdn.net/sunjunior");
    iTrayIcon->showMessage(tr("关于作者"), tr("若想进一步了解作者本人信息，请关注个人技术博客http://blog.csdn.net/sunjunior!")
                           .arg(timerIntervalSpinBox->value()));
    QDesktopServices::openUrl(QUrl(OnlineHelpURL));
}



QSize QFtpClient::sizeHint() const
{
    return QSize(640, 480);
}

void QFtpClient::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();

}

void QFtpClient::iconActivated(QSystemTrayIcon::ActivationReason reason)
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

void QFtpClient::slotRestart()
{
//    switch( QMessageBox::information( this, tr("重启提示"),
//                                     tr("确认重启客户端？"),tr("确定"), tr("取消"),0, 1))
//    {
//    case 0:
//        qApp->exit(773);
//        break;
//    case 1:
//    default:
//        break;
//    }
     qApp->exit(773);

}

void QFtpClient::getAllScripts(const QString &filesPath)
{
    QDir dir(filesPath);
    QStringList filters;
    filters << "*.txt";

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    m_filesBox->clear();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        m_filesBox->addItem(fileInfo.fileName());

    }

}

void QFtpClient::changeFilePath()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("打开目录"),
                                                         zarpathEdit->text(),
                                                         QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if(!dirpath.isEmpty())
    {
       zarpathEdit->setText(dirpath);

    }
}

void QFtpClient::changeScriptPath()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("打开目录"),
                                                         scriptpathEdit->text(),
                                                         QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if(!dirpath.isEmpty())
    {
       scriptpathEdit->setText(dirpath);

    }
    getAllScripts(dirpath);

}

void QFtpClient::getRunTime()
{

    int time_Diff = time->elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    //以下方法是将ms转为s
    int total_seconds = time_Diff / 1000;
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600)/60;
    int seconds = total_seconds % 60;
    iTrayIcon->showMessage(tr("提示"), tr("本程序在自动取版本时间周期内持续时长为%1小时%2分钟%3秒.")
                           .arg(hours).arg(minutes).arg(seconds));

}

void QFtpClient::getAllIp()
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

void QFtpClient::slotUpdateList()
{

    if ( ftp && (ftp->state() == QFtp::Connected
       || ftp->state() == QFtp::LoggedIn))
    {
        fileList->clear();
        isDirectory.clear();
        ftp->list();
    }
   // iTrayIcon->showMessage(tr("提示"), tr("更新当前FTP目录:%1")
   //                        .arg(dateTime));

}

void QFtpClient::timerinit()
{
    if (isDownloadCheckBox->isChecked())
    {
        if (!m_zarTimer->isActive())
        {
            m_zarTimer->start();//20s自动取一次版本
        }

        if (!m_updateTimer->isActive())
        {
            m_updateTimer->start();//每分钟更新一下当前目录和当前日期，防止zar文件无法下载时，日期无法更新问题
        }

        if (m_dateTimer->isActive())//版本周期一般为16～24小时之内
        {
            m_dateTimer->stop();
        }

        if (!m_calendarTimer->isActive()) //与m_dateTimer周期一致，防止每日构建失败之后，无法取第二天版本
        {
            m_calendarTimer->start();
        }
        if (!iconTimer->isActive())
        {
            iconTimer->start();
        }

        isZarDir = false;

        dateTime = currentDateEdit->text();//获取每日构建版本日期
        iTrayIcon->showMessage(tr("提示"), tr("准备获取%1每日构建版本.")
                               .arg(dateTime));

    }
    else
    {
        qDebug() << "telnet device directly" << endl;
        emit done();

    }   

}

void QFtpClient::slotAutoDownload()
{
    if(!ftp)
    {
        connectOrDisconnect();
        return;
    }

    if (ftp->state() == QFtp::Unconnected)
    {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
        fileList->setEnabled(false);
        connectButton->setText(tr("已经断开"));
        iTrayIcon->showMessage(tr("提示"),tr("已断开连接 %1.")
                               .arg(ftpServerLineEdit->text()));
        return;
    }

    qDebug()<< "connected" << endl;
    if(QString::compare(dateTime, currentDateEdit->text(), Qt::CaseInsensitive))
    {
        dateTime = currentDateEdit->text();
        iTrayIcon->showMessage(tr("提示"), tr("重新获取%1每日构建版本.")
                               .arg(dateTime));
    }

    QString filename = fileList->headerItem()->text(0);

    QTreeWidgetItemIterator iter(fileList->headerItem());
    while (*iter)
    {
        filename = (*iter)->text(0);

        if((filename == tr("V2_1每日构建")) || (filename.indexOf(dateTime) != -1 ))
        {

            isZarDir = (filename.indexOf(dateTime) != -1 )? true:false; //匹配是否在每日构建目录中的zar文件

            filename = QString::fromLatin1(filename.toLocal8Bit());
            if(isDirectory.value(filename))
            {

                fileList->clear();
                isDirectory.clear();
                ftp->cd(filename);
                ftp->list();
                setCursor(Qt::WaitCursor);
            }

            break;
        }


        if( isZarDir && filename.indexOf("zxr10") != -1)
        {
            qDebug() << "iszarfile:" << isZarDir << "filename:" << QString::fromLocal8Bit(filename.toLatin1())<< endl;
            savedFilePath = zarpathEdit->text();
            savedFilePath = savedFilePath + "/" + filename;
            qDebug()<<savedFilePath <<endl;
            file = new QFile(savedFilePath);
            if(file->exists(savedFilePath))
            {
               file->remove();
            }
            if (!file->open(QIODevice::WriteOnly))
            {
                iTrayIcon->showMessage(tr("提示"), tr("无法保存文件至%1: %2.")
                                       .arg(savedFilePath).arg(file->errorString()));
                file->close();
                return;
            }

            m_zarTimer->stop();
            m_updateTimer->stop();
            m_dateTimer->start();
            isNotifyTelnet = false;
            connect(ftp, SIGNAL(stateChanged(int)), this, SLOT(slotChangeState()));//下载时中断连接会导致程序重新初始化

            ftp->get(filename, file);
            iTrayIcon->showMessage(tr("提示"),tr("正在下载zar文件至 %1")
                                   .arg(savedFilePath));
            progressDialog->setLabelText(tr("正在下载zar文件至 %1").arg(savedFilePath));
            if (progressDialog->exec() == QDialog::Accepted)
            {
                qApp->processEvents(QEventLoop::AllEvents, 100);

            }


            break;
        }

        ++iter;
    }
}

void QFtpClient::slotChangeState()
{

    if (ftp && ftp->state() == QFtp::Unconnected)
    {
        qDebug()<< "slot changestate unconnected"<<endl;
        ftp->clearPendingCommands();
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
        fileList->setEnabled(false);
        connectButton->setText(tr("已经断开"));
        iTrayIcon->showMessage(tr("提示"),tr("已断开连接 %1.")
                               .arg(ftpServerLineEdit->text()));

        progressDialog->cancel();
        m_zarTimer->start();
        m_updateTimer->start();
        m_dateTimer->stop();
        iTrayIcon->showMessage(tr("提示"), tr("下载zar文件失败，程序将20s之后自动重连服务器!"));


        if(file)
        {
            file->close();
            file->remove();
            file->deleteLater();
        }
    }
}

void QFtpClient::cancelDownload()
{
    if(file)
    {
        file->deleteLater();
        file->close();
        file->remove();

    }

    if(ftp)
    {
        ftp->clearPendingCommands();
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
    }

    fileList->setEnabled(false);
    connectButton->setText(tr("已经断开"));
    iTrayIcon->showMessage(tr("提示"),tr("已断开连接 %1.")
                           .arg(ftpServerLineEdit->text()));
    progressDialog->hide();
    m_zarTimer->start();
    m_updateTimer->start();
    m_dateTimer->stop();
    iTrayIcon->showMessage(tr("提示"), tr("下载zar文件失败，程序将20s之后自动重连服务器!"));

}

void QFtpClient::connectOrDisconnect()
{
    if (ftp) {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
        fileList->setEnabled(false);
        connectButton->setText(tr("已经断开"));
        iTrayIcon->showMessage(tr("提示"),tr("已断开连接 %1.")
                               .arg(ftpServerLineEdit->text()));
        setCursor(Qt::ArrowCursor);
        return;
    }

    setCursor(Qt::WaitCursor);

    ftp = new QFtp(this);
    connect(ftp, SIGNAL(commandFinished(int, bool)),
            this, SLOT(ftpCommandFinished(int, bool)));


    connect(ftp, SIGNAL(listInfo(const QUrlInfo &)),
            this, SLOT(addToList(const QUrlInfo &)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)),
            this, SLOT(updateDataTransferProgress(qint64, qint64)));


    fileList->clear();
    isDirectory.clear();

    QUrl url(ftpServerLineEdit->text());
    if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp"))
    {
        ftp->connectToHost(ftpServerLineEdit->text(),ftpPortLineEdit->text().toInt());
        loginTimer->start(10000);
        ftp->login(loginnameLineEdit->text(), passwordLineEdit->text());

    }
    else
    {
        ftp->connectToHost(url.host(), url.port(ftpPortLineEdit->text().toInt()));
        loginTimer->start(10000);
        if (!url.userName().isEmpty())
        {
            ftp->login(loginnameLineEdit->text(), passwordLineEdit->text());

        }

        else
        {
            ftp->login();

        }

        if (!url.path().isEmpty())
            ftp->cd(url.path());
    }
    qDebug()<<ftp->currentCommand() <<"connectOrDisconnect "<<  ftp->state()<<endl;
    fileList->setEnabled(true);
    connectButton->setText(tr("已连接上"));
    iTrayIcon->showMessage(tr("提示"),tr("正在登录服务器 %1...")
                           .arg(ftpServerLineEdit->text()));

}

void QFtpClient::timeDone()
{
    if(ftp == NULL)
    {
        return;
    }
    qDebug() << ftp->currentCommand() << " timeDone " <<  ftp->state()<<endl;
    if( (ftp->currentCommand() == QFtp::ConnectToHost)
      && ((ftp->state() == QFtp::Unconnected ) || (ftp->state() == QFtp::Connecting )))
    {
        qDebug()<< " timeDone " <<endl;
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
        fileList->setEnabled(false);
        connectButton->setText(tr("已经断开"));
        iTrayIcon->showMessage(tr("提示"),tr("服务器连接超时,已断开连接 %1.")
                               .arg(ftpServerLineEdit->text()));
        setCursor(Qt::ArrowCursor);
        if(loginTimer->isActive())
        {
           loginTimer->stop();
        }
    }

}


void QFtpClient::ftpCommandFinished(int, bool error)
{
    setCursor(Qt::ArrowCursor);

    if (ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if (error)
        {
            if(loginTimer->isActive())
            {
               loginTimer->stop();
            }

            iTrayIcon->showMessage(tr("提示"),tr("无法连接到服务器%1."
                                        "请检查服务器地址是否正确 ")
                                     .arg(ftpServerLineEdit->text()));
            connectOrDisconnect();
            return;
        }

        iTrayIcon->showMessage(tr("提示"),tr("已经连接至服务器 %1.")
                               .arg(ftpServerLineEdit->text()));
        fileList->setFocus();


        return;
    }

    if (ftp->currentCommand() == QFtp::Login)
    {
        ftp->list();
        if(loginTimer->isActive())
        {
           loginTimer->stop();
        }
    }

    if (ftp->currentCommand() == QFtp::Get)
    {
        if(loginTimer->isActive())
        {
           loginTimer->stop();
        }
        if (error)
        {
            iTrayIcon->showMessage(tr("提示"),tr("取消下载文件 %1.")
                                   .arg(file->fileName()));
            file->close();
            file->remove();
        }
        else
        {
            iTrayIcon->showMessage(tr("提示"),tr("已下载文件至指定目录 %1.")
                                   .arg(savedFilePath));
            file->close();
            if(!isNotifyTelnet)
            {
                qDebug() << "ftp state:" << ftp->state() << endl;
                if (ftp && ftp->state() == QFtp::LoggedIn)
                {
                    QString log = tr("下载zar文件不完整，程序将20s之后重新在服务器%1下载zar文件.")
                            .arg(ftpServerLineEdit->text());
                    iTrayIcon->showMessage(tr("提示"), log);
                    recordLog(log);
                    m_zarTimer->start();
                    m_updateTimer->start();
                    m_dateTimer->stop();


                }

            }

        }
        progressDialog->hide();


    }
    else if (ftp->currentCommand() == QFtp::List)
    {
        if (isDirectory.isEmpty())
        {
            fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<目录为空>")));
            fileList->setEnabled(false);
        }
    }

}


void QFtpClient::addToList(const QUrlInfo &urlInfo)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    double size = urlInfo.size()/(1048576.0);
    if (urlInfo.name() == tr(".") || urlInfo.name() == tr("..")) //排除当前目录和上一层目录等特殊目录
    {
        return;
    }
    item->setText(0, QString::fromLocal8Bit(urlInfo.name().toLatin1()));
    if(urlInfo.isDir())
    {
       item->setText(1, QString::number((int)size));
    }else
    {
       item->setText(1, QString::number(size, 'f', 2));
    }
    item->setText(2, urlInfo.owner());
    item->setText(3, urlInfo.group());
    item->setText(4, urlInfo.lastModified().toString("yyyy年MM月dd日"));


    QPixmap pixmap(urlInfo.isDir() ? ":/images/dir.png" : ":/images/file.png");
    item->setIcon(0, pixmap);

    isDirectory[urlInfo.name()] = urlInfo.isDir();
    fileList->addTopLevelItem(item);
    if (!fileList->currentItem())
    {
        fileList->setCurrentItem(fileList->topLevelItem(0));
        fileList->setEnabled(true);
    }

}

void QFtpClient::updateDataTransferProgress(qint64 readBytes,
                                            qint64 totalBytes)
{
    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(readBytes);
    qDebug() << "readBytes:" << readBytes << "totalBytes:" << totalBytes << endl;
    if (totalBytes == readBytes)
    {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
        file->close();
        file->deleteLater();
        fileList->setEnabled(false);
        connectButton->setText(tr("已经断开"));
        QString log = tr("成功下载zar文件，已断开ftp连接 %1.")
                .arg(ftpServerLineEdit->text());
        iTrayIcon->showMessage(tr("提示"), log);
        recordLog(log);
        setCursor(Qt::ArrowCursor);
        isNotifyTelnet = true; //QT FTP协议本身原因，发现有时readBytes与totalBytes不相等，不相等时，重新下载zar文件
        emit done();
    }
}

void QFtpClient::delaymsec(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

}


void QFtpClient::slotTelnetDevice()
{

    if(deviceIpCombox->count() == 0)
    {
        iTrayIcon->showMessage(tr("提示"),tr("设备IP为空，请添加需要自动配置的设备IP!"));
        QTimer::singleShot(15000, this, SLOT(slotTelnetDevice()));
        return;
    }

    loginTimes = 0;//登录失败次数
    output = "";
    isconnected = false;
    isAutoConfig = false; //是否重启设备
    deviceindex = 0;
    deviceIpCombox->setCurrentIndex(deviceindex);
    iTrayIcon->showMessage(tr("提示"),tr("准备telnet设备:%1!").arg(deviceIpCombox->currentText()));
    if (telnetTimer->isActive())
    {
        telnetTimer->stop();
    }
    connect(telnetTimer, SIGNAL(timeout()),this, SLOT(slotstartConnection()));//10秒自动重新连接

    QTimer::singleShot(3000, this, SLOT(slotstartConnection()));
}


void QFtpClient::slotstartConnection()
{
    if(deviceIpCombox->currentText().isEmpty())
    {
        time->restart(); //执行完了，则重新开始计算程序开始取版本时间，以ms为单位
        if (iconTimer->isActive())
        {
            iTrayIcon->setIcon(QIcon(":/images/traydark.png"));
            iconTimer->stop();
        }

        return;
    }

    iTrayIcon->showMessage(tr("提示"),tr("正在telnet设备%1...").arg(deviceIpCombox->currentText()));
    m_pSocket->abort();
    m_pSocket->connectToHost(QHostAddress(deviceIpCombox->currentText()), 23);
    qApp->processEvents(QEventLoop::AllEvents, 100);
    connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotdisplayError(QAbstractSocket::SocketError)), Qt::AutoConnection);
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(slotreadMessage()));
    connect(m_pSocket, SIGNAL(connected()), this, SLOT(slotgetStatus()));
    connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(slotgetStatus()));

    if (m_pSocket->waitForConnected(10000))//等待10s，是否超时
    {
        QString log = tr("第%1次telnet设备:%2成功，准备执行cli命令！")
                .arg(QString::number(loginTimes + 1)).arg(deviceIpCombox->currentText());
        iTrayIcon->showMessage(tr("提示"), log);
        recordLog(log);
        if (telnetTimer->isActive())
        {
            telnetTimer->stop();
        }
        autologin();//自动登录
        runCmdsByTelnet();

    }
    else
    {
        m_pSocket->close();

        if (loginTimes < 10) //连接等待超时后，尝试telnet总次数为10次
        {
            QString log = tr("第%1次telnet设备:%2失败，10秒自动重新连接！")
                    .arg(QString::number(loginTimes + 1)).arg(deviceIpCombox->currentText());

            qDebug()<< log << endl;
            iTrayIcon->showMessage(tr("提示"), log);
            recordLog(log);
            telnetTimer->start(10000);
            loginTimes++;
        }
        else
        {
            if (telnetTimer->isActive())
            {
                telnetTimer->stop();
            }
            QString log = tr("telnet设备%1失败，无法登录设备执行cli命令！").arg(deviceIpCombox->currentText());
            qDebug()<< log << endl;
            iTrayIcon->showMessage(tr("提示"), log);
            recordLog(log);

            telnetNextDevice();//登录下一台设备执行cli命令，如果有的话

        }

    }

}

void QFtpClient::telnetNextDevice()
{
    if(deviceindex < deviceIpCombox->count())
    {
        loginTimes = 0;//登录失败次数
        output = "";
        isconnected = false;
        isAutoConfig = false; //是否重启设备
        deviceindex++;
        deviceIpCombox->setCurrentIndex(deviceindex);
        QTimer::singleShot(3000, this, SLOT(slotstartConnection()));
    }

}

void QFtpClient::autologin()
{
    if (deviceUsernameLineEdit->text().isEmpty() || devicePasswordLineEdit->text().isEmpty())
    {
        QString log = tr("Telnet设备%1用户名或密码为空,请确认！").arg(deviceIpCombox->currentText());
        qDebug()<< log << endl;
        iTrayIcon->showMessage(tr("提示"), log);
        recordLog(log);
        return;

    }
    this->runCmds(deviceUsernameLineEdit->text());
    this->runCmds(devicePasswordLineEdit->text());
    this->runCmds("en");
    this->runCmds("zxr10");

}

void QFtpClient::runCmdsByTelnet()
{
    if (!isAutoConfig && isRebootCheckBox->isChecked())
    {
        qDebug()<< "restart Device" << endl;
        delaymsec(2000);
        QString log = tr("执行cli命令，重启设备:%1！").arg(deviceIpCombox->currentText());
        qDebug()<< log << endl;
        iTrayIcon->showMessage(tr("提示"), log);
        recordLog(log);
        this->runCmds("reload");
        this->runCmds("yes");
        isAutoConfig = true;
        telnetTimer->start(360000);//重启后等待6分钟

    }
    else
    {
        delaymsec(2000);
        QString log = tr("登录设备:%1执行cli配置命令！").arg(deviceIpCombox->currentText());
        qDebug()<< log << endl;
        iTrayIcon->showMessage(tr("提示"), log);
        recordLog(log);
        this->runCmds("olleh");
        this->runCmds("diag execute mp master cmd g_boardConfigDebug=1");//防止板类型与实际不一致情况
        this->runCmds("!");
        QString currentPath = "";
        for (fileindex = 0; fileindex < m_filesBox->count(); fileindex++)
        { 
            if(m_filesBox->itemText(fileindex).indexOf(deviceIpCombox->currentText()) != -1)
            {
                currentPath =  scriptpathEdit->text() + "/" + m_filesBox->itemText(fileindex);
                m_filesBox->setCurrentIndex(fileindex);
                QFile configfile(currentPath);
                this->runCmds(&configfile);
                qApp->processEvents(QEventLoop::AllEvents, 100);
                delaymsec(5000);
            }

        }
        recordLog(output);
        telnetNextDevice();//执行完了指令，再登录另外一台设备，如果有的话
    }

}


void QFtpClient::recordLog(const QString& log)
{
    QFile logfile(tr("自动配置工具运行记录.log"));
    if(!logfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        return;
    }
    QString logDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//记录当前时间;
    QTextStream out(&logfile);
    out << logDateTime <<":" << log << endl;
    logfile.close();

}

void QFtpClient::slotdisplayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
    {
        return;
    }
    isconnected = false;
    m_pSocket->close();
    loginTimes++;
    iTrayIcon->showMessage(tr("提示"),tr("连接产生如下错误: %1.")
               .arg(m_pSocket->errorString()));


    output = "";

}



void QFtpClient::slotgetStatus()
{

    switch(m_pSocket->state())
    {
    case QAbstractSocket::UnconnectedState:
    {
        iTrayIcon->showMessage(tr("提示"), tr("Telnet已断开连接！"));
        isconnected = false;
        output = "";
        break;
    }

    case QAbstractSocket::ConnectingState:
    {
        iTrayIcon->showMessage(tr("提示"),tr("Telnet正在连接中..."));
        isconnected = false;
        break;
    }

    case QAbstractSocket::ConnectedState:
    {
        iTrayIcon->showMessage(tr("提示"), tr("Telnet连接已建立！"));
        isconnected = true;

        break;
    }

    default:
        break;
    }

}

void QFtpClient::runCmds(const QString& cmds)
{
    this->cmds = cmds;
    sendMessage();
}

void QFtpClient::runCmds(QFile *pfile)
{
    if (pfile == NULL)
    {
       qDebug()<< tr("默认脚本文件错误") << endl;
       return;
    }

    if(!pfile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString log = tr("配置文件无法打开！请确认当前目录下文件是否存在!");
        iTrayIcon->showMessage(tr("提示"), log);
        recordLog(log);
        return;
    }
    QTextStream in(pfile);
    this->cmds = in.readAll();
    pfile->close();
    sendMessage();
}


void QFtpClient::slotreadMessage()
{
    output += m_pSocket->readAll();
    qDebug()<< output << endl;
}


void QFtpClient::sendMessage()
{
    if (this->cmds.isEmpty())
    {
        return;
    }

    QStringList cmdList = cmds.split("\n");
    foreach(QString cmd, cmdList)
    {
        qDebug() <<"cmd:" << cmd << endl;
        cmd += "\n";
        if (!m_pSocket->isValid())
        {
            return;
        }
        m_pSocket->write(cmd.toAscii());
        delaymsec(500);
    }

}
