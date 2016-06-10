#ifndef CTELNET_H_INCLUDED
#define CTELNET_H_INCLUDED

#include <QWidget>
#include <QSystemTrayIcon>

class QLabel;
class QComboBox;
class QProcess;
class QPushButton;
class QTextEdit;
class QTimer;
class QMessageBox;
class QAction;
class QMenu;
class QLineEdit;

class QConfigMenu;

class QTime;
class QStatusBar;


class CTelnet: public QWidget
{
    Q_OBJECT
public:
    CTelnet();
    ~CTelnet();
private slots:
    void linecardRunClicked();
    void deviceRunClicked();
    void readOutput();
    void timerDone();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showConfigMenu(void);
    void getAllIp(void);
    void getRunTime(void);
private:
    void closeEvent(QCloseEvent *event);


private:
    QProcess *cmd;
    QString output;
    QLabel *deviceIpLabel;
    QComboBox *deviceIpCombox;
    QPushButton *deviceRunBtn;

    QLabel *linecardIpLabel;
    QComboBox *linecardIpCombox;
    QPushButton *linecardRunBtn;

    QLabel *outputLabel;
    QTextEdit *outputEdit;
    QTimer *timer;

    QSystemTrayIcon *iTrayIcon;
    QMenu *iTraymenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QPushButton *configBtn;

    QConfigMenu *configMenu;

    QTime *time;
    QStatusBar *statusBar;

};

#endif // CTELNET_H_INCLUDED
