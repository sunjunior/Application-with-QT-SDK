#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T22:08:07
#
#-------------------------------------------------

QT       += core gui
QT      += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IpLookup
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    +=

RESOURCES += \
    resource.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
