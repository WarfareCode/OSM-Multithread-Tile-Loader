QT -= svg
QT += core gui network widgets
CONFIG += c++11
VERSION = 3.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

#message("Дата сборки= $$BUILD_DATE")
BUILD_DATE = $$system( date /t ) $$system( time /t )
BUILD_DATE = $$member(BUILD_DATE,0)_$$member(BUILD_DATE,1)
DEFINES += BUILD_DATE=\\\"$$BUILD_DATE\\\"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    SettingsKeys.h \
    widgets/checkerwidget.h \
    model/imagechecker.h \
    model/imagedowloader.h \
    widgets/loaderwidget.h \
    widgets/mainwidget.h \
    model/logtitudesrepository.h \
    widgets/longtitudewidget.h

SOURCES += \
        widgets/checkerwidget.cpp \
        model/imagechecker.cpp \
        model/imagedowloader.cpp \
        widgets/loaderwidget.cpp \
        main.cpp \
        widgets/mainwidget.cpp \
        model/logtitudesrepository.cpp \
        widgets/longtitudewidget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    astyle.astylerc


