QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0
QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

            CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

    SOURCES += \
       main.cpp \
    mediaplayer.cpp

               HEADERS += \
               mediaplayer.h

# Default rules for deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
                                             else: unix:!android: target.path = /opt/$${TARGET}/bin
      !isEmpty(target.path): INSTALLS += target

            RESOURCES += \
    images.qrc \
       sounds.qrc

FORMS +=

DISTFILES += \
    images/ghost.png \
    images/pacman.png \
    images/pacman2.png


