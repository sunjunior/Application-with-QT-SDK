

QT += widgets

OTHER_FILES += \
    qt_zh_CN.qm \
    360safe_zh.ts \
    360safe_zh.qm \
    360safe_en.ts \
    360safe_en.qm \
    360safe.suo \
    360safe.rc

RC_FILE = 360safe.rc

HEADERS += \
    util.h \
    tool_button.h \
    title_widget.h \
    system_tray.h \
    skin_widget.h \
    skinwidget.h \
    setting_dialog.h \
    resource.h \
    push_button.h \
    main_widget.h \
    main_menu.h \
    content_widget.h \
    common.h \
    clabel.h \
    character_widget.h \
    change_skin_widget.h \
    about_us.h

SOURCES += \
    util.cpp \
    tool_button.cpp \
    title_widget.cpp \
    system_tray.cpp \
    skin_widget.cpp \
    skinwidget.cpp \
    setting_dialog.cpp \
    push_button.cpp \
    main_widget.cpp \
    main_menu.cpp \
    main.cpp \
    content_widget.cpp \
    clabel.cpp \
    character_widget.cpp \
    change_skin_widget.cpp \
    about_us.cpp

RESOURCES += \
    QRC.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
