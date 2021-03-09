QT = core gui sql
QT += serialport
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += openssl
CONFIG += serialport
LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_5_15_0_MinGW_64_bit-Debug/debug/SimConnect.dll"

win64 {
    INCLUDEPATH += "C:/Projects/Build Output/include/"
    LIBS += -L"C:/Program Files/OpenSSL-Win64/lib" -lubsec
    INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
    CONFIG(debug, debug|release) {


        LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_5_15_0_MinGW_64_bit-Debug/debug/SimConnect.dll"

    }
    else {
         LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_5_15_0_MinGW_64_bit-Debug/debug/SimConnect.dll"
    }
}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    sources/ConnectWorker.cpp \
    sources/InputWorker.cpp \
    sources/SerialPort.cpp \
    sources/Set.cpp \
    sources/inputenum.cpp \
    sources/inputmapper.cpp \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/optionsmenu.cpp \
    sources/InputSwitchHandler.cpp

HEADERS += \
    headers/ConnectWorker.h \
    headers/InputWorker.h \
    headers/Set.h \
    headers/mainwindow.h \
    headers/SerialPort.hpp \
    headers/SimConnect.h \
    headers/optionsmenu.h \
    headers/InputSwitchHandler.h \
    headers/inputenum.h \
    headers/InputMapper.h \


INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
FORMS += \
    mainwindow.ui \
    optionsmenu.ui

TRANSLATIONS += \
    Bitsanddroidsgui_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    bitsanddroids.qdocconf \
    doc/bits-and-droids-flight-sim-connector.index \
    html/bits-and-droids-flight-sim-connector.index \
    styles.css

