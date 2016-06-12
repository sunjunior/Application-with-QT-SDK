#include "qbatcmd.h"
#include <QtWidgets>


QBatCmd::QBatCmd(QWidget *parent) :
    QWidget(parent)
{
    m_pathLabel = new QLabel(tr("脚本所在目录:"));
    m_pathEdit = new QLineEdit;
    m_pathEdit->setText(QCoreApplication::applicationDirPath());
    m_pathEdit->setReadOnly(true);    
    m_pathBtn = new QPushButton(tr("更改路径"));

    QHBoxLayout *hlayout1  = new QHBoxLayout;
    hlayout1->addWidget(m_pathLabel);
    hlayout1->addWidget(m_pathEdit);
    hlayout1->addWidget(m_pathBtn);

    m_runShLabel = new QLabel(tr("当前运行脚本:"));
    m_runShLabel->setFixedWidth(100);
    m_runShBox = new QComboBox;

    QHBoxLayout *hlayout2  = new QHBoxLayout;
    hlayout2->addWidget(m_runShLabel);
    hlayout2->addWidget(m_runShBox);

    m_xlsxLabel = new QLabel(tr("Xls生成目录:"));
    m_xlsxLineEdit = new QLineEdit;
    m_xlsxLineEdit->setText(qApp->applicationDirPath());
    m_xlsxLineEdit->setReadOnly(true);
    m_xlsxPathBtn = new QPushButton(tr("更改路径"));

    QHBoxLayout *hlayout3  = new QHBoxLayout;
    hlayout3->addWidget(m_xlsxLabel);
    hlayout3->addWidget(m_xlsxLineEdit);
    hlayout3->addWidget(m_xlsxPathBtn);

    m_dateStartLabel = new QLabel(tr("起始日期:"));
    m_dateStartEdit = new QLineEdit(tr("2014-05-25"));
    m_dateStartEdit->setReadOnly(true);

    m_dateFinishLabel = new QLabel(tr("结束日期:"));
    m_dateFinishEdit = new QLineEdit(tr("2014-06-25"));
    m_dateFinishEdit->setReadOnly(true);

    m_dateChangeBtn = new QPushButton(tr("更改日期"));
    connect(m_dateChangeBtn, SIGNAL(clicked()), this, SLOT(getChangeDate()));

    QHBoxLayout *hlayout4  = new QHBoxLayout;
    hlayout4->addWidget(m_dateStartLabel);
    hlayout4->addWidget(m_dateStartEdit);
    hlayout4->addWidget(m_dateFinishLabel);
    hlayout4->addWidget(m_dateFinishEdit);
    hlayout4->addWidget(m_dateChangeBtn);


    isEmailCheckBox = new QCheckBox(tr("发送邮件给各项目经理及科长"), this);
    isEmailCheckBox->setChecked(true);
    isAutoRunCheckBox = new QCheckBox(tr("自动运行代码统计工具"), this);
    isAutoRunCheckBox->setChecked(false);
    isAutoRunCheckBox->setToolTip(tr("勾选此选项则每月16号和26号自动执行代码变更统计脚本，无需人工干预!"));
    connect(isAutoRunCheckBox, SIGNAL(stateChanged(int)), this, SLOT(initialize()));

    QHBoxLayout *hlayout5  = new QHBoxLayout;
    hlayout5->addWidget(isEmailCheckBox);
    hlayout5->addWidget(isAutoRunCheckBox);


    m_showLabel = new QLabel(tr("执行结果："));
    m_showEdit = new QTextEdit;
    m_showEdit->setReadOnly(true);
    m_runBtn = new QPushButton(tr("运行脚本"));
    m_runBtn->setFixedWidth(100);
    m_runBtn->setEnabled(false);
    m_stopBtn = new QPushButton(tr("停止运行"));
    m_stopBtn->setFixedWidth(100);
    m_stopBtn->setEnabled(false);

    QHBoxLayout *hlayout6  = new QHBoxLayout;
    hlayout6->addWidget(m_runBtn);
    hlayout6->addWidget(m_stopBtn);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout4);
    vlayout->addLayout(hlayout5);

    vlayout->addWidget(m_showLabel);
    vlayout->addWidget(m_showEdit);
    vlayout->addLayout(hlayout6);
    setLayout(vlayout);
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    setWindowIcon(QIcon(":/images/app.png"));
    setWindowTitle(tr("代码变更统计工具"));
    setFixedSize(640,480);


    iTrayIcon = new QSystemTrayIcon;
    iTrayIcon->setIcon(QIcon(":/images/tray.png"));
    iTrayIcon->setVisible(true);
    iTrayIcon->setToolTip(tr("代码变更统计工具"));

    openScriptAction = new QAction(tr("打开脚本目录"), this);
    openScriptAction->setIcon(QIcon(":/images/opendir.png"));
    connect(openScriptAction, SIGNAL(triggered()), this, SLOT(openScriptDir()));
    restoreAction = new QAction(tr("显示主界面"), this);
    restoreAction->setIcon(QIcon(":/images/menu.png"));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("退出"), this);
    quitAction->setIcon(QIcon(":/images/close.png"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    iTraymenu=new QMenu();
    iTraymenu->addAction(openScriptAction);
    iTraymenu->addAction(restoreAction);
    iTraymenu->addAction(quitAction);
    iTraymenu->addSeparator();
    iTrayIcon->setContextMenu(iTraymenu);
    connect(iTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    output = "";
    connect(m_pathBtn, SIGNAL(clicked()), this, SLOT(getBatFilePath()));
    connect(m_xlsxPathBtn, SIGNAL(clicked()), this, SLOT(getXlsxFilePath()));
    connect(m_showEdit, SIGNAL(textChanged()), this, SLOT(autoScroll()));
    connect(m_stopBtn, SIGNAL(clicked()), this, SLOT(stopClicked()));
    connect(m_runBtn, SIGNAL(clicked()), this, SLOT(startClicked()));

    xlsTimer = new QTimer;
    xlsTimer->setInterval(15000);//每15s轮询一次
    connect(xlsTimer, SIGNAL(timeout()), this, SLOT(getXlsFiles()));


    currentDateTime = new QDateTime;

    updateTimer = new QTimer;
    updateTimer->setInterval(86400000);


    connect(updateTimer, SIGNAL(timeout()), this, SLOT(autoUpdateRunBats()));

    connect(this, SIGNAL(autorun()), m_runBtn, SIGNAL(clicked()));

    initialize();

}

QBatCmd::~QBatCmd()
{

}


void QBatCmd::iconActivated(QSystemTrayIcon::ActivationReason reason)
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

void QBatCmd::openScriptDir(void)
{
    QString path = m_pathEdit->text();//获取ZAR文件存放目录

    QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));

}

void QBatCmd::initialize()
{
    if (xlsTimer->isActive())
    {
        xlsTimer->stop();
        qDebug() << "timer stop" << endl;
    }


    if (isAutoRunCheckBox->isChecked())
    {
        autoUpdateRunBats();//自动运行脚本
        if (!updateTimer->isActive())
        {
            updateTimer->start();
        }

        m_runBtn->setEnabled(false);
        m_stopBtn->setEnabled(false);
        iTrayIcon->showMessage(tr("提示"), tr("程序目前处在自动模式下，每月16号或者26号自动进行代码变更统计，无需人工干预！"));

    }
    else
    {
        if (updateTimer->isActive())
        {
            updateTimer->stop();
        }

        iTrayIcon->showMessage(tr("提示"), tr("程序目前处在手工模式下，需要点击运行按钮才可进行代码变更统计！"));
        m_runBtn->setEnabled(true);
        m_stopBtn->setEnabled(true);

    }

}

void QBatCmd::autoUpdateRunBats(void)
{
    output += tr("此程序于:") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +tr("刷新了日期");//记录当前时间;
    output += "\n";
    m_showEdit->setText(output);
    QString currentDay =  currentDateTime->currentDateTime().toString("dd");//刷新日期，是否为每月的16号或者26号
    if (currentDay.indexOf("16") != -1)
    {
        QString newDate = currentDateTime->currentDateTime().addDays(-1).toString("yyyy-MM-dd");
        qDebug() << newDate << endl;
        if(QString::compare(newDate, m_dateFinishEdit->text(), Qt::CaseInsensitive) > 0)
        {
            m_dateStartEdit->setText(m_dateFinishEdit->text());
            m_dateFinishEdit->setText(newDate);
        }

        autoMakeRakefile();//更新rakefile文件
        emit autorun();
    }
    if (currentDay.indexOf("26") != -1)
    {
        QString newDate = currentDateTime->currentDateTime().addDays(-1).toString("yyyy-MM-dd");
        qDebug() << newDate << endl;
        m_dateFinishEdit->setText(newDate);
        autoMakeRakefile();//更新rakefile文件
        emit autorun();
    }
}

void QBatCmd::getChangeDate(void)
{
    bool ok = false;
    QRegExp rxlen("^(\\d{4}-\\d{2}-\\d{2})$");

    QString text = QInputDialog::getText(this, tr("修改变更日期"),
                                         tr("起始日期:"), QLineEdit::Normal,
                                         m_dateStartEdit->text(), &ok);
    if (ok && !text.isEmpty() && text.indexOf(rxlen) != -1)
    {
        m_dateStartEdit->setText(text);

    }


    text = QInputDialog::getText(this, tr("修改变更日期"),
                                 tr("结束日期:"), QLineEdit::Normal,
                                 m_dateFinishEdit->text(), &ok);

    if (ok && !text.isEmpty() && text.indexOf(rxlen) != -1)
    {
        m_dateFinishEdit->setText(text);
    }

    autoMakeRakefile();//更新rakefile文件

}


void QBatCmd::autoMakeRakefile(void)
{
    QString filepath = m_xlsxLineEdit->text() + "/" + "rakefile";
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "# -*- encoding: utf-8 -*-" << endl
        << "require 'rspec/zte'" << endl
        << "# ----------------------------------------------------------" << endl
        <<  tr("#counter信息设置") << endl
        << "# ----------------------------------------------------------" << endl
        << "$opts = {" << endl
        << ":start_time   => Time.parse('"  << m_dateStartEdit->text() + "')," << endl
        << ":finish_time  => Time.parse('" << m_dateFinishEdit->text() <<  "')" << endl
        << "}"  << endl
        << "$logger = XLogger.new 'counter(' + Time.datestamp + ').log'" << endl;

    file.close();

}

void QBatCmd::autoGetRakefileDate()
{
    QString filepath = m_xlsxLineEdit->text() + "/" + "rakefile";
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QRegExp rxlen("(\\d{4}-\\d{2}-\\d{2}).*(\\d{4}-\\d{2}-\\d{2})");
    int pos = rxlen.indexIn(content);
    if (pos > -1)
    {
        QString startDate = rxlen.cap(1);
        QString finishDate = rxlen.cap(2);
        m_dateStartEdit->setText(startDate);
        m_dateFinishEdit->setText(finishDate);
    }

}



void QBatCmd::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

void QBatCmd::startClicked(void)
{
    if (m_runShBox->count() == 0)
    {
        qDebug()<< tr("没有找到任何可执行脚本！") << endl;
        return;
    }

    index = 0;
    m_runShBox->setCurrentIndex(index);
    output = "";
    m_runBtn->setEnabled(false);
    qDebug() << tr("脚本开始执行！") << endl;
    m_showEdit->clear();
    runBatFile();

}

void QBatCmd::runBatFile()
{

    if (index >= m_runShBox->count())
    {
        if (xlsTimer->isActive())
        {
            xlsTimer->stop();
        }
        output += tr("*********************统计脚本执行完成!*********************");
        output += "\n";
        m_showEdit->setText(output);
        m_runBtn->setEnabled(true);

        //清理EXCEL进程
        output += tr("清理EXCEL进程");
        output += "\n";
        m_showEdit->setText(output);
        QString cmd = m_xlsxLineEdit->text() + "/" + tr("杀死所有EXCEL进程.exe");
        qDebug() << cmd << endl;
        if (QFile::exists(cmd))
        {
            if (true == QProcess::startDetached(cmd))
            {
                qApp->processEvents(QEventLoop::AllEvents, 100);
            }

        }
        delaymsec(8000);
        output += tr("将6100与CSS项目中统计结果放入TIME项目中");
        output += "\n";
        m_showEdit->setText(output);
        cmd = m_xlsxLineEdit->text() + "/" + tr("ParseExcel.exe");
        qDebug() << cmd << endl;
        if (QFile::exists(cmd))
        {
            if (true == QProcess::startDetached(cmd))
            {
                qApp->processEvents(QEventLoop::AllEvents, 100);
            }

        }
        delaymsec(30000);

        //查看是否已经存在codecount_result.rar,存在则删除

        cmd = m_xlsxLineEdit->text() + "/codecount_result.rar";

        if (QFile::exists(cmd))
        {
            output += tr("删除已有的压缩包文件codecount_result.rar");
            output += "\n";
            m_showEdit->setText(output);
            QFile::remove(cmd);
        }
        //将统计结果压缩为codecount_result.rar
        output += tr("将统计结果压缩为codecount_result.rar");
        output += "\n";
        m_showEdit->setText(output);
        cmd = "7z.exe a -y " + m_xlsxLineEdit->text() + "/codecount_result.rar " + m_xlsxLineEdit->text() + tr("/*.xlsx");
        qDebug() << cmd << endl;
        QProcess::execute(cmd);

        if (isEmailCheckBox->isChecked())
        {
            //发送邮件,先处理STMP.ini
            autoChangeEmailIni();
            delaymsec(2000);
            output += tr("发送邮件给各位项目经理及科长");
            output += "\n";
            m_showEdit->setText(output);
            cmd = m_xlsxLineEdit->text() + "/SendEmail/SendSMTP.exe /s " + m_xlsxLineEdit->text() + "/SendEmail/SendSMTP.ini";
            qDebug() << cmd << endl;
            QProcess::execute(cmd);
        }

        return;
    }


    QString input = m_pathEdit->text() + "/" + m_runShBox->itemText(index);
    m_runShBox->setCurrentIndex(index);
    output += tr("正在执行当前脚本:");
    output +=  input;
    output += "\n";
    m_showEdit->setText(output);

    QStringList environment = QProcess::systemEnvironment();
    if (true == QProcess::startDetached(input, environment, m_pathEdit->text()))
    {
        qApp->processEvents(QEventLoop::AllEvents, 100);
    }


    isfounded = false;
    if (!xlsTimer->isActive())
    {
        qDebug() << "timer start" << endl;
        xlsTimer->start();
    }

}

void QBatCmd::autoChangeEmailIni()
{

    QString filepath = m_xlsxLineEdit->text() + "/SendEmail/SendSMTP.ini";
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);
    out << "[SendSMTP]" << endl
        <<  "Host=szsmtp06.zte.com.cn"  << endl
        <<  "Port=25" << endl
        <<  "Timeout=15" << endl
        <<  "UserID=SunJun10145578" <<endl
        <<  tr("Password=挐姈摏无") <<endl
        <<  "SaveLog=1"  << endl
        <<  tr("FromName=部门代码变更统计") << endl
        <<  "FromAddr=10.10.202.151" << endl
        <<  "To=sun.jun37@zte.com.cn" << endl
        <<  "ReplyTo=" <<endl
        <<  "cc=li.xiaowei@zte.com.cn" << endl
        <<  "bcc=" << endl
        <<  "Subject=" + m_dateStartEdit->text() + tr("至") + m_dateFinishEdit->text() + tr("代码变更统计情况") << endl
        <<  "Body=1" << endl
        <<  "FileCount=1" << endl
        <<  "BodyFile=" + m_xlsxLineEdit->text().replace("/","\\") + "\\SendEmail\\SendSMTPBody.rtf" <<endl
        <<  "File0=" + m_xlsxLineEdit->text().replace("/","\\") + "\\codecount_result.rar" << endl
        <<  "File1=" + m_xlsxLineEdit->text().replace("/","\\") + "\\SendEmail\\SendSMTPBody.rtf" << endl;

    file.close();

}

void QBatCmd::delaymsec(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

}



void QBatCmd::getXlsFiles(void)
{
    qDebug() << "getXlsFiles timedone" << endl;
    QRegExp rxFileName("(^[0-9]+)\\.xlsx");
    QDir dir(m_xlsxLineEdit->text());
    QStringList filters;
    filters << "*.xlsx";

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName().indexOf(rxFileName) != -1)
        {
            qDebug() << "fileInfo:" << fileInfo.fileName() << endl;
            batXlsFile = fileInfo.fileName();
            isfounded = true;
            break;
        }

    }

    if(isfounded)
    {
        QRegExp rxFileName("(.*)\\.bat");
        // qDebug() << "batXlsFile: " << batXlsFile << endl;
        oldName = m_xlsxLineEdit->text() + "/" + batXlsFile;
        if (QFile::exists(oldName))
        {
            int pos = rxFileName.indexIn(m_runShBox->itemText(index));
            if (pos > -1)
            {
                newName = rxFileName.cap(1) + "-" +  batXlsFile;
            }
            newName = m_xlsxLineEdit->text() + "/" + newName;
            //             qDebug() << "oldName:" << oldName << endl;
            //             qDebug() << "newName:" << newName << endl;
            output = output + oldName + tr("成功改名为:") + newName + "\n";
            if ( QFile::rename(oldName, newName))
            {
                index++;
                output += tr("执行下一脚本文件\n");
                m_showEdit->setText(output);
                QTimer::singleShot(5000, this, SLOT(runBatFile()));
            }

        }

    }

}

void QBatCmd::stopClicked(void)
{

    if (xlsTimer->isActive())
    {
        xlsTimer->stop();
    }
    if (!m_runBtn->isEnabled())
    {
        m_runBtn->setEnabled(true);
    }

}

void QBatCmd::getXlsxFilePath(void)
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("打开目录"),
                                                        m_xlsxLineEdit->text(),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);

    if(!dirpath.isEmpty())
    {
        m_xlsxLineEdit->setText(dirpath);
        autoGetRakefileDate();

    }
}



void QBatCmd::getBatFilePath(void)
{

    QString dirpath = QFileDialog::getExistingDirectory(this, tr("打开目录"),
                                                        m_pathEdit->text(),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if(!dirpath.isEmpty())
    {
        m_pathEdit->setText(dirpath);

    }
    QDir dir(dirpath);
    QStringList filters;
    filters << "*.bat";

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    m_runShBox->clear();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        m_runShBox->addItem(fileInfo.fileName());

    }

}



void QBatCmd::autoScroll()
{
    m_showEdit->moveCursor(QTextCursor::End);

}


