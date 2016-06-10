#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include "qbatcmd.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QTranslator trans;
    trans.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&trans);

    QBatCmd w;
    w.show();
    qApp->processEvents(QEventLoop::AllEvents, 100);
    return app.exec();
}
