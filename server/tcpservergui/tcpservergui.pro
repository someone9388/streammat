#-------------------------------------------------
#
# Project created by QtCreator 2014-04-30T21:53:27
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpservergui
TEMPLATE = app
INCLUDEPATH += f:/opencv/build/include
LIBS += -Lf:/opencv/build/x86/vc11/lib \
    -lopencv_core248d \
    -lopencv_highgui248d \
    -lopencv_imgproc248d \
    -lopencv_features2d248d \
    -lopencv_calib3d248d

SOURCES += main.cpp\
        dialog.cpp \
    mytcpserver.cpp

HEADERS  += dialog.h \
    mytcpserver.h

FORMS    += dialog.ui

RESOURCES += \
    myres.qrc
