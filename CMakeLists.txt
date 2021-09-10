cmake_minimum_required(VERSION 3.20)
project(Bitsanddroidsgui)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/QtWindows/5.15.0/mingw81_64/lib/cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        Network
        Serialport
        REQUIRED)

include_directories(.)
include_directories(dual)
include_directories(headers)
include_directories(Inputs)
include_directories(outputs)

include_directories(settings)
include_directories(sources)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(Bitsanddroidsgui

        dual/dualworker.cpp
        dual/dualworker.h
        headers/constants.h
        headers/Engine.h

        headers/mainwindow.h

        headers/range.h
        headers/SerialPort.hpp
        headers/SerialReader.h
        headers/set.h
        headers/settingsranges.h
        headers/SimConnect.h
        Inputs/inputenum.cpp
        Inputs/inputenum.h
        Inputs/inputmapper.cpp
        Inputs/InputMapper.h
        Inputs/InputSwitchHandler.cpp
        Inputs/InputSwitchHandler.h
        Inputs/InputWorker.cpp
        Inputs/InputWorker.h

        outputs/activeoutputs.cpp
        outputs/activeoutputs.h
        outputs/output.cpp
        outputs/output.h
        outputs/outputbundle.cpp
        outputs/outputbundle.h
        outputs/outputenum.cpp
        outputs/outputEnum.h
        outputs/outputhandler.cpp
        outputs/outputhandler.h
        outputs/outputmapper.cpp
        outputs/outputmapper.h
        outputs/outputworker.cpp
        outputs/outputworker.h
        outputs/set.cpp
        outputs/set.h
        outputs/sethandler.cpp
        outputs/sethandler.h
        settings/formbuilder.cpp
        settings/formbuilder.h
        settings/optionsmenu.cpp
        settings/optionsmenu.h
        settings/optionsmenu.ui
        settings/outputmenu.cpp
        settings/outputmenu.h
        settings/settingshandler.cpp
        settings/settingshandler.h
        settings/settingsranges.cpp
        settings/settingsranges.h
        settings/optionsmenu.ui
        settings/outputmenu.ui
        sources/Engine.cpp
        sources/mainwindow.ui
        sources/main.cpp
        sources/mainwindow.cpp
        sources/range.cpp
        sources/SerialPort.cpp
        sources/SerialReader.cpp
        qcustomplot.cpp
        qcustomplot.h
        settings/coordinates.cpp
        settings/coordinates.h

     )
target_link_libraries(Bitsanddroidsgui
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        Qt5::Charts
        Qt5::Network
        Qt5::SerialPort
        )
if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
