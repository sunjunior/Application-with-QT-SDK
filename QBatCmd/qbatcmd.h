#ifndef QBATCMD_H
#define QBATCMD_H

#include <QWidget>
#include <QSystemTrayIcon>
class QLineEdit;
class QTextEdit;
class QPushButton;
class QLabel;
class QComboBox;
class QTimer;
class QCheckBox;
class QDateTime;



class QBatCmd : public QWidget
{
    Q_OBJECT

public:
    explicit QBatCmd(QWidget *parent = 0);
    ~QBatCmd();
signals:
    void autorun(void);

private slots:
    void initialize(void);
    void getChangeDate(void);
    void getBatFilePath(void);
    void getXlsxFilePath(void);
    void autoScroll(void);
    void stopClicked(void);
    void runBatFile(void);
    void startClicked(void);
    void getXlsFiles(void);
    void openScriptDir(void);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void autoUpdateRunBats(void);

private:

     void closeEvent(QCloseEvent *event);
     void delaymsec(int msec);
     void autoGetRakefileDate(void); //自动从rakefile文件中读取日期范围
     void autoMakeRakefile(void);
     void autoChangeEmailIni(void); //处理邮件ini文件


private:

    QLabel     *m_pathLabel;
    QLineEdit  *m_pathEdit;
    QPushButton *m_pathBtn;

    QLabel    *m_runShLabel;
    QComboBox *m_runShBox;

    QLabel   *m_xlsxLabel;
    QLineEdit   *m_xlsxLineEdit;
    QPushButton *m_xlsxPathBtn;


    QLabel *m_dateStartLabel;
    QLineEdit *m_dateStartEdit;
    QLabel *m_dateFinishLabel;
    QLineEdit *m_dateFinishEdit;
    QPushButton *m_dateChangeBtn;

    QCheckBox *isEmailCheckBox;

    QLabel   *m_showLabel;
    QTextEdit *m_showEdit;

    QPushButton *m_runBtn;
    QPushButton *m_stopBtn;


    QSystemTrayIcon *iTrayIcon;
    QMenu *iTraymenu;
    QAction *openScriptAction;
    QAction *restoreAction;
    QAction *quitAction;


    QString output;
    QTimer *xlsTimer;
    QTimer *updateTimer;
    QDateTime *currentDateTime;//更新当前日期
    QCheckBox *isAutoRunCheckBox;


    int index;
    bool isfounded;
    QString batXlsFile;
    QString oldName;
    QString newName;


};

#endif // QBATCMD_H
