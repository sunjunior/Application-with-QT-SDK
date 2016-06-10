#include "simpleplayer.h"
#include <QtWidgets/QApplication>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include "single_application.h"

static void associateFileTypes(const QStringList &fileTypes)
{
	QString displayName = QGuiApplication::applicationDisplayName();
	QString filePath = QCoreApplication::applicationFilePath();
	QString fileName = QFileInfo(filePath).fileName();

	QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\Applications\\" + fileName, QSettings::NativeFormat);
	settings.setValue("FriendlyAppName", displayName);

	settings.beginGroup("SupportedTypes");
	foreach(const QString& fileType, fileTypes)
		settings.setValue(fileType, QString());
	settings.endGroup();

	settings.beginGroup("shell");
	settings.beginGroup("open");
	settings.setValue("FriendlyAppName", displayName);
	settings.beginGroup("Command");
	settings.setValue(".", QChar('"') + QDir::toNativeSeparators(filePath) + QString("\" \"%1\""));
}

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	//QApplication app(argc, argv);

    SingleApplication app(argc, argv, "simpleplayer");

    //associateFileTypes(QStringList(".mp3"));
	const QStringList arguments = QCoreApplication::arguments();

    SimplePlayer* player = new SimplePlayer;

	if (arguments.size() > 1)
        player->Simple_Open_Music(arguments.at(1));


    player->move((QApplication::desktop()->width() - player->width()) / 2,
                 (QApplication::desktop()->height() - player->height()) / 2);
    player->show();



	
	return app.exec();
}
