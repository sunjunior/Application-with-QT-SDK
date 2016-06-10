
HEADERS += \
    qftpclient.h

SOURCES += \
    qftpclient.cpp \
    main.cpp
RESOURCES    += ftp.qrc

QT           += network
RC_FILE += app.rc

OTHER_FILES +=

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/QFtpClient/bin
    INSTALLS += target
}
