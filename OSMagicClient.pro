#-------------------------------------------------
#
# Project created by QtCreator 2020-04-17T17:54:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

TARGET = OSMagicClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(UI/ui.pri)
include(Service/service.pri)
include(Dao/dao.pri)
CONFIG += c++11

win32{
contains(QT_ARCH, i386):{
INCLUDEPATH += = $$PWD/3rd/curl/include \
                 $$PWD/3rd/klvideowidget/include \
                 $$PWD/3rd/ffmpeg/x64/include
LIBS += $$PWD/3rd/curl/lib/windows/win32/libcurl-d_imp.lib \
        ws2_32.lib
}else{
INCLUDEPATH += = $$PWD/3rd/curl/include \
                 $$PWD/3rd/klvideowidget/include \
                 $$PWD/3rd/ffmpeg/x64/include

CONFIG(debug,debug|release){
LIBS += -LD:/OpenSSL-Win64/lib/VC \
        -L$$PWD/3rd/ffmpeg/x64/lib \
        $$PWD/3rd/curl/lib/windows/win64/libcurl-d_imp.lib \
        $$PWD/3rd/klvideowidget/x64/debug/klvideowidget.lib \
        libcrypto64MD.lib libssl64MD.lib ws2_32.lib \
        avcodec.lib avformat.lib avutil.lib swresample.lib swscale.lib
}else{
LIBS += -LD:/OpenSSL-Win64/lib/VC \
        -L$$PWD/3rd/ffmpeg/x64/lib \
        $$PWD/3rd/curl/lib/windows/win64/libcurl-d_imp.lib \
        $$PWD/3rd/klvideowidget/x64/release/klvideowidget.lib \
        libcrypto64MD.lib libssl64MD.lib ws2_32.lib \
        avcodec.lib avformat.lib avutil.lib swresample.lib swscale.lib
}
}
}

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
