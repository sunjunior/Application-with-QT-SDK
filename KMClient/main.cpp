#include <QApplication>
#include <QtCore>
#include "tcpclient.h"
#include <QSplashScreen>




int main(int argc, char *argv[])
{



     QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
     QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
     QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);
    app.setFont( QFont( "System", 14) );

        QSplashScreen *splash = new QSplashScreen;
        splash->setPixmap(QPixmap(":/images/splash.png"));
        splash->show();
        splash->showMessage(QObject::tr("Welcome to ..."));

        qApp->processEvents();

      //  qDebug("Time elapsed: %d ms", t.currentTime());

            TcpClient tcpclient;

            tcpclient.show();
          QTimer::singleShot(5000,splash,SLOT(close()));
          QTimer::singleShot(2000,&tcpclient,SLOT(show()));
           splash->finish(&tcpclient);




     return app.exec();
}


