#include "widget.h"
#include <QApplication>
#include <QTimer>
#include <QPixmap>
#include <QSplashScreen>
#include <QFont>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFont font("simsun", 16);
    app.setFont(font);
    QPixmap pixmap(":/resources/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QObject::tr("☆※欢迎进入生活一点通※☆"),Qt::AlignHCenter | Qt::AlignBottom, Qt::blue);
    QTimer::singleShot(5000, &splash, SLOT(close()));
    Widget widget;
    QTimer::singleShot(5000, &widget, SLOT(show()));




    return app.exec();
}
