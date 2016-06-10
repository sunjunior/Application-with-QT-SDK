QT += widgets
QT += multimedia
QT += network

SOURCES += \
    main.cpp \
    nofocusdelegate.cpp \
    single_application.cpp \
    simplelabel.cpp \
    simpleplayer.cpp \
    simplevolbutton.cpp \
    simpleaboutform.cpp \
    simplemusicbutton.cpp

HEADERS += \
    resource.h \
    nofocusdelegate.h \
    single_application.h \
    songinfo.h \
    simplelabel.h \
    simpleplayer.h \
    simplevolbutton.h \
    simpleaboutform.h \
    simplemusicbutton.h

RESOURCES += \
    simpleplayer.qrc

RC_FILE += SimplePlayer.rc
