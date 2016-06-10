#include <QtCore/QCoreApplication>
#include <QTextCodec>
#include <QFile>
#include "qtelnet.h"


int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QTelnet telnet;
    telnet.telnetToHost("192.192.192.198","23");
    telnet.runCmds("who");
    telnet.runCmds("who");
    telnet.runCmds("en");
    telnet.runCmds("zxr10");
//    QFile file("script.txt");
//    telnet.runCmds(&file);

    return app.exec();

}
