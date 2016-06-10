#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QProcess>
#include <QTranslator>
#include "qftpclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFont font("simhei", 10);
    app.setFont(font);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QTranslator trans;
    trans.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&trans);


    QFtpClient client;
    client.show();
    int ret = app.exec();

    if (ret == 773)
    {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }

    return ret;

}


