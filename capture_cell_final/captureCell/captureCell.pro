QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AlphaBetaPruner.cpp \
    GuiBack.cpp \
    GuiFront.cpp \
    Player.cpp \
    StartGameDialog.cpp \
    Utils.cpp \
    main.cpp \

HEADERS += \
    AlphaBetaPruner.h \
    GuiBack.h \
    GuiFront.h \
    Player.h \
    StartGameDialog.h \
    Utils.h \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    StartGameDialog.ui
