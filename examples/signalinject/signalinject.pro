QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        config.cpp \
        eject/injector.cpp \
        eject/internal/voidvalue.cpp \
        eject/private/injectorprivate.cpp \
        logbase.cpp \
        main.cpp \
        worker.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    config.h \
    eject/injector.h \
    eject/internal/voidvalue.h \
    eject/private/injectorprivate.h \
    ilog.h \
    iloggable.h \
    logbase.h \
    worker.h
