#-------------------------------------------------
#
# Project created by QtCreator 2018-02-28T16:34:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TermographyApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    videocapture.cpp \
    imageconfiguration.cpp

HEADERS  += mainwindow.h \
    videocapture.h \
    imageconfiguration.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lseek
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lseek
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lseek

INCLUDEPATH += $$PWD/../../../../../usr/local/include/seek
DEPENDPATH += $$PWD/../../../../../usr/local/include/seek


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/release/ -lopencv_highgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/debug/ -lopencv_highgui
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../../usr/include/opencv
DEPENDPATH += $$PWD/../../../../../usr/include/opencv

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/release/ -lopencv_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/debug/ -lopencv_core
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../../usr/include/opencv2/core
DEPENDPATH += $$PWD/../../../../../usr/include/opencv2/core

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/release/ -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/debug/ -lopencv_imgproc
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/arm-linux-gnueabihf/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../../usr/include/opencv2/imgproc
DEPENDPATH += $$PWD/../../../../../usr/include/opencv2/imgproc
