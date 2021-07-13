QT = core gui sql
QT += serialport
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += openssl
CONFIG += serialport
LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_5_15_0_MinGW_64_bit-Release/release/SimConnect.dll"
RC_FILE = BitsAndDroidsGui.rc
win64 {
    INCLUDEPATH += "C:/Projects/Build Output/include/"
    LIBS += -L"C:/Program Files/OpenSSL-Win64/lib" -lubsec
    INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
    INCLUDEPATH += "C:/MSFS SDK/WASM/wasi-sysroot/include"
    INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include"
    CONFIG(debug, debug|release) {
        LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_5_15_0_MinGW_64_bit-Release/release/SimConnect.dll"
    }
    else {
         LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_5_15_0_MinGW_64_bit-Release/release/SimConnect.dll"

    }
}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    Inputs/InputSwitchHandler.cpp \
    Inputs/InputWorker.cpp \
    Inputs/inputenum.cpp \
    Inputs/inputmapper.cpp \
    settings/formbuilder.cpp \
    settings/optionsmenu.cpp \
    settings/settingsranges.cpp \
    sources/ConnectWorker.cpp \
    sources/Engine.cpp \
    sources/SerialPort.cpp \
    sources/Set.cpp \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/radioworker.cpp \
    sources/range.cpp

HEADERS += \
    Inputs/InputMapper.h \
    Inputs/InputSwitchHandler.h \
    Inputs/InputWorker.h \
    Inputs/inputenum.h \
    headers/Engine.h \
    headers/ConnectWorker.h \
    headers/Set.h \
    headers/constants.h \
    headers/mainwindow.h \
    headers/SerialPort.hpp \
    headers/SimConnect.h \
    headers/radioworker.h \
    headers/range.h \
    settings/formbuilder.h \
    settings/optionsmenu.h \
    settings/settingsranges.h

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
    bitsAndDroids \
    bitsanddroids.qdocconf \
    doc/bits-and-droids-flight-sim-connector.index \
    fonts/DSEG7Classic-Bold.ttf \
    fonts/DSEG7Classic-Regular.ttf \
    html/bits-and-droids-flight-sim-connector.index \
    styles.css

RESOURCES += \
    Resources.qrc
