#include <QApplication>
#include <QtCore>
#include "tcpdialog.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resource);

     QApplication::addLibraryPath("./plugins"); //数据编码文件
     QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
     QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
     QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

     QApplication app(argc, argv);
     app.setFont( QFont( "System", 14) );

     TcpDialog dialog;
     dialog.show();


    return app.exec();
}
