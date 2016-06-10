#include <QApplication>
#include <QFont>
#include <QTranslator>
#include <QTextCodec>
#include "main_widget.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
  //  QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
  //  QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	QTranslator translator;  
	translator.load(QString(":/qm/qt_zh_CN"));  
	app.installTranslator(&translator);  

	QTranslator translator_zh;  
	translator_zh.load(QString(":/qm/360safe_zh.qm"));  
	app.installTranslator(&translator_zh);  

    QFont font("simsun", 10);
	app.setFont(font);

    MainWidget main_widget;
    main_widget.showWidget();

    return app.exec();
}
