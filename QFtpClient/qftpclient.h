#ifndef QFTPCLIENT_H
#define QFTPCLIENT_H

#include <QHash>
#include <QWidget>

#include <QSystemTrayIcon>
#include <QTcpSocket>

class QDialogButtonBox;
class QFile;
class QFtp;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QProgressDialog;
class QPushButton;
class QUrlInfo;
class QStatusBar;
class QTimer;
class QSpinBox;
class QAction;
class QMenu;
class QCheckBox;
class QCalendarWidget;
class QDate;
class QComboBox;




class QFtpClient : public QWidget
{
    Q_OBJECT

public:
    QFtpClient(QWidget *parent = 0);
    QSize sizeHint() const;

signals:
    void done();
    void dateChanged(const QDate &date );


private slots:
    void slotAutoIconChange(void);//每秒改变tray icon图标
    void connectOrDisconnect();
    void cancelDownload();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void ftpCommandFinished(int commandId, bool error);
    void addToList(const QUrlInfo &urlInfo);
    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void slotupdateDateTimer(void);
    void slotOpenZarDir(void);
    void slotContactUs(void);
    void slotDeviceIpAdd(void);
    void selectedDateChanged(void);
    void getZarFileDate(void);
    void changeFilePath(void);
    void changeScriptPath(void);
    void timeDone(void);
    void getAllIp(void);
    void getRunTime(void);
    void slotAutoDownload(void);
    void timerinit(void);
    void slotUpdateDate(void);
    void slotUpdateList(void);
    void slotChangeState(void);
    void slotRestart(void);

    void slotTelnetDevice();
    void slotreadMessage();
    void slotgetStatus();
    void slotdisplayError(QAbstractSocket::SocketError socketError);
    void slotstartConnection(void);

private:
    void closeEvent(QCloseEvent *event);
    void delaymsec(int msec);
    void getAllScripts(const QString &filesPath);
    void telnetNextDevice();
    void sendMessage();
    void runCmds(const QString& cmds);//执行命令
    void runCmds(QFile *pfile); //加载默认脚本
    bool isConnected() const { return  isconnected;} //返回连接是否成功
    void recordLog(const QString& log);
    void runCmdsByTelnet(void);
    void autologin(void);


private:
    QLabel *ftpServerLabel;
    QLineEdit *ftpServerLineEdit;
    QLabel *ftpPortLabel;
    QLineEdit *ftpPortLineEdit;
    QLabel *timerIntervalLabel;
    QSpinBox *timerIntervalSpinBox;
    QPushButton *connectButton;
    QLabel *loginnameLabel;
    QLineEdit *loginnameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;

    QLabel *currentDateLabel;
    QLineEdit *currentDateEdit;
    QCalendarWidget *calendar;
    QPushButton *changeDateBtn;

    QCheckBox *isDownloadCheckBox;
    QCheckBox *isRebootCheckBox;


    QLabel *zarPathLabel;
    QLineEdit  *zarpathEdit;
    QPushButton *zarpathBtn;

    QLabel *scriptPathLabel;
    QLineEdit  *scriptpathEdit;
    QPushButton *scriptpathBtn;

    QLabel    *m_filesLabel;
    QComboBox *m_filesBox;

    QLabel *deviceIpLabel;
    QComboBox *deviceIpCombox;
    QPushButton *deviceIpAddBtn;
    QLabel *telnetPortLabel;
    QLineEdit *telnetPortLineEdit;
    QLabel *deviceUsernameLabel;
    QLineEdit *deviceUsernameLineEdit;
    QLabel *devicePasswordLabel;
    QLineEdit *devicePasswordLineEdit;
    QStatusBar *statusBar;
    QTreeWidget *fileList;

    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;

    QHash<QString, bool> isDirectory;
    QString savedFilePath;
    QFtp *ftp;
    QFile *file;
    QTimer *loginTimer;

    QTimer *m_updateTimer;//每30s更新一次FTP文件目录
    QTimer *m_zarTimer;//每隔10s轮询一次
    QTimer *m_dateTimer;//每隔24小时轮询一次
    QTimer *m_calendarTimer; //每隔24小时更新一次,防止某日每日构建没有出来导致无法取第二天的构建版本
    QString dateTime;//每日构建版本日期

    bool isZarDir;
    bool isNotifyTelnet;

    QSystemTrayIcon *iTrayIcon;
    QMenu *iTraymenu;
    QAction *minimizeAction;
    QAction *maximumAction;
    QAction *freetimeAction;
    QAction *restartAction;
    QAction *openZarDirAction;
    QAction *restoreAction;
    QAction *contactAction;
    QAction *quitAction;
    QTime *time; //获取当前程序运行时间

    QTcpSocket *m_pSocket; //设备是否运行正常，是否能telnet通
    QString output;
    QString cmds;
    QTimer *telnetTimer;
    qint8 loginTimes;
    int fileindex;
    int deviceindex;

    bool isconnected;
    bool isAutoConfig;

    bool isIconDark;
    QTimer *iconTimer; //每秒动态改变Icon图标


};

#endif // QFTPCLIENT_H
