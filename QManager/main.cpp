#include <QtWidgets>
#include <QTextCodec>
#include <QTranslator>
#include "widget.h"
#include "connection.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFont font("simhei", 12);
    app.setFont(font);
    // 必须在创建数据库之前使用，不然无法在数据库中使用中文
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    QTranslator trans;
    trans.load(":/qm/qt_zh_CN.qm");
    app.installTranslator(&trans);

    if(!createConnection() || !createXml()) return 0;

    LoginDialog dlg;
    Widget w;
    if (dlg.exec() == QDialog::Accepted)
    {
        w.show();
        return app.exec();
    }
    else
    {
        return 0;
    }
}

