#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QTimer>
#include <QSplashScreen>
#include <QTimer>

#include "CTelnet.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QFont font("simmei",10,QFont::Normal,FALSE);
    app.setFont(font);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QObject::tr("☆※欢迎进入Telnet主程序※☆"),Qt::AlignHCenter | Qt::AlignBottom, Qt::blue);
    QTimer::singleShot(3000, &splash, SLOT(close()));
    CTelnet telnet;
    QTimer::singleShot(3000, &telnet, SLOT(show()));

    return app.exec();
}
