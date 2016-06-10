#ifndef QCONFIGMENU_H
#define QCONFIGMENU_H

#include <QWidget>

class QLabel;
class QComboBox;
class QProcess;
class QPushButton;
class QLineEdit;
class QTimer;
class QMessageBox;
class QGroupBox;
class QTextEdit;
class QComboBox;
class QSpinBox;

class QClient;

class QConfigMenu : public QWidget
{
    Q_OBJECT

public:
     QConfigMenu();
    ~QConfigMenu();
private slots:
    void on_runBtn();
    void on_showConfigBtn();
    void closeEvent(QCloseEvent *);
    void on_mkConfigBtn();
private:
     QGroupBox *m_groupBox;
     QLabel *m_hostIPlabel;
     QLineEdit *m_hostIPEdit;
     QLabel *m_loopbacklabel;
     QLineEdit *m_loopbackEdit;
     QLabel *m_peerloopbacklabel;
     QLineEdit *m_peerloopbackEdit;
     QLabel *m_vlanlabel;
     QLineEdit *m_vlanEdit;
     QLabel *m_peervlanlabel;
     QLineEdit *m_peervlanEdit;
     QLabel *m_peerMacLabel;
     QLineEdit *m_peerMacEdit;
     QLabel *m_cepSlotNoLabel;
     QSpinBox *m_cepSlotNoBox;
     QLabel *m_cepPortNoLabel;
     QComboBox *m_cepPortNoBox;
     QLabel *m_geSlotNoLabel;
     QSpinBox *m_geSlotNoBox;
     QLabel *m_gePortNoLabel;
     QComboBox *m_gePortNoBox;
     QPushButton *m_showConfigBtn;
     QPushButton *m_runBtn;

     QString output;
     QProcess *cmd;
     QTextEdit *m_runConfigEdit;

     QPushButton *mkConfigBtn;

     QGroupBox *m_configBox;

     QClient *client;



};

#endif // QCONFIGMENU_H
