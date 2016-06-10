#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QNetworkAccessManager;
class QNetworkReply;
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QComboBox;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void query(const QString &content);

signals:
    void finished(bool bOK, QString ip, QString area);

private slots:
     void onQueryFinished(bool bOK, QString content, QString area);
     void onReplyFinished(QNetworkReply *reply);
     void onClickedFinished();
     void onTimerProc(void);
     void readAllData();
private:
    QLineEdit *lineEdit;
    QPushButton *queryBtn;
    QLabel *typeLabel;
    QComboBox *queryBox;
    QTextEdit *textEdit;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray output;
};

#endif // WIDGET_H
