#include "qwincomm.h"
#include <QApplication>
#include <QTextCodec>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QWinComm win;
    win.show();

    return app.exec();

}
