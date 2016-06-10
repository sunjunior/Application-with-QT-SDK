#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QTextEdit;
class QProcess;

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_prjpathBtn_clicked();
    void readOutput();

    void on_runBtn_clicked();
    void autoScroll(void);

private:
    void closeEvent(QCloseEvent *event);


    Ui::Widget *ui;
    QTextEdit *m_showTextEdit;
    QProcess *cmd;
    QString output;

};

#endif // WIDGET_H
