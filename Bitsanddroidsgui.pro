QT = core gui serialport network
QT += printsupport
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += object_parallel_to_source


CONFIG += c++17
CONFIG += openssl
MOC_DIR= $$PWD/objects
OBJECTS_DIR= $$PWD/objects

# using shell_path() to correct path depending on platform
# escaping quotes and backslashes for file paths
copydata.commands = $(COPY_FILE) \"$$shell_path($$PWD\\dist)\" \"$$shell_path($$OUT_PWD)\\release\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

copydata.commands = $(COPY_DIR) \"$$shell_path($$PWD\\events)\" \"C:\\Users\\Dave Riedel\\AppData\Local\\Packages\\Microsoft.FlightSimulator_8wekyb3d8bbwe\\LocalCache\\Packages\\Community\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

copydata.commands = $(COPY_DIR) \"$$shell_path($$PWD\\events)\" \"$$shell_path($$OUT_PWD)\\release\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata


LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_6_2_0_MinGW_64_bit-Release/release/SimConnect.dll"
RC_FILE = BitsAndDroidsGui.rc
win64 {
    INCLUDEPATH += "C:/Projects/Build Output/include/"
    LIBS += -L"C:/Program Files/OpenSSL-Win64/lib" -lubsec
    INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
    INCLUDEPATH += "C:/MSFS SDK/WASM/wasi-sysroot/include"
    INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include"
    CONFIG(debug, debug|release) {
        LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_6_2_0_MinGW_64_bit-Release/release/SimConnect.dll"
    }
    else {
         LIBS += "C:/Users/Dave Riedel/Documents/build-Bitsanddroidsgui-Desktop_Qt_6_2_0_MinGW_64_bit-Release/release/SimConnect.dll"

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
    dual/dualworker.cpp \
    events/eventwindow.cpp \
    outputs/activeoutputs.cpp \
    outputs/output.cpp \
    outputs/outputbundle.cpp \
    outputs/outputenum.cpp \
    outputs/outputhandler.cpp \
    outputs/outputmapper.cpp \
    outputs/outputworker.cpp \
    outputs/set.cpp \
    outputs/sethandler.cpp \
    settings/formBuilder.cpp \
    settings/optionsmenu.cpp \
    settings/outputmenu.cpp \
    settings/coordinates.cpp \
    settings/settingshandler.cpp \
    settings/settingsranges.cpp \
    sources/Engine.cpp \
    sources/SerialPort.cpp \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/range.cpp

HEADERS += \
    Inputs/InputMapper.h \
    Inputs/InputSwitchHandler.h \
    Inputs/InputWorker.h \
    Inputs/inputenum.h \
    dual/dualworker.h \
    events/eventwindow.h \
    headers/Engine.h \
    headers/constants.h \
    headers/mainwindow.h \
    headers/SerialPort.hpp \
    headers/SimConnect.h \
    headers/range.h \
    outputs/activeoutputs.h \
    outputs/output.h \
    outputs/outputbundle.h \
    outputs/outputenum.h \
    outputs/outputhandler.h \
    outputs/outputmapper.h \
    outputs/outputworker.h \
    outputs/set.h \
    outputs/sethandler.h \
    settings/formBuilder.h \
    settings/optionsmenu.h \
    settings/outputmenu.h \
    settings/coordinates.h \
    settings/settingshandler.h \
    settings/settingsranges.h


INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
FORMS += \
    events/eventwindow.ui \
    sources/mainwindow.ui \
    settings/optionsmenu.ui \
    settings/outputmenu.ui

TRANSLATIONS += \
    Bitsanddroidsgui_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    bitsAndDroids \
    bitsanddroids.qdocconf \
    config/config.xml \
    doc/bits-and-droids-flight-sim-connector.index \
    fonts/DSEG7Classic-Bold.ttf \
    fonts/DSEG7Classic-Regular.ttf \
    outputs.json \
    html/bits-and-droids-flight-sim-connector.index \
    images/refreshicon.png \
    packages/package.xml \
    styles.css

RESOURCES += \
    Resources.qrc\
    outputs.json



