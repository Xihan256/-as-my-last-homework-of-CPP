QT       += core gui multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientwindow.cpp \
    gamemanager.cpp \
    gamewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mypoint.cpp \
    netchoicewidget.cpp \
    onlinegamewindow.cpp \
    serverwindow.cpp \
    webmsgwidget.cpp

HEADERS += \
    clientwindow.h \
    gamemanager.h \
    gamewindow.h \
    mainwindow.h \
    mypoint.h \
    netchoicewidget.h \
    onlinegamewindow.h \
    serverwindow.h \
    webmsgwidget.h

FORMS += \
    clientwindow.ui \
    mainwindow.ui \
    serverwindow.ui \
    webmsgwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
