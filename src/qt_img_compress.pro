QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cfiledialog.cpp \
    main.cpp \
    mainwindow.cpp \
    postfile.cpp \
    downloadfile.cpp

HEADERS += \
    cfiledialog.h \
    mainwindow.h \
    postfile.h \
    downloadfile.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    qt_img_compress_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
RC_ICONS = icon.png
