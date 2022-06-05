# Generated from Bitsanddroidsgui.pro.

cmake_minimum_required(VERSION 3.16)
project(BitsAndDroidsConnector VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Gui Network PrintSupport SerialPort Test REQUIRED)

set(PROJECT_SOURCES

        main.cpp
        constants.h

        handlers/InputSwitchHandler.cpp
        handlers/InputSwitchHandler.h
        handlers/sethandler.cpp handlers/sethandler.h

        workers/dualworker.cpp workers/dualworker.h
        workers/InputWorker.cpp workers/InputWorker.h
        workers/InputMapper.h workers/inputmapper.cpp
        workers/outputworker.cpp workers/outputworker.h

        elements/ModeIndexCheckbox.cpp elements/ModeIndexCheckbox.h
        elements/ModeIndexCombobox.cpp elements/ModeIndexCombobox.h
        elements/mcheckbox.cpp elements/mcheckbox.h

        enums/ModeEnum.h
        enums/inputenum.cpp enums/inputenum.h
        enums/outputenum.cpp enums/outputEnum.h

        models/events/Event.h
        models/events/EventDatatype.cpp models/events/EventDatatype.h
        models/events/EventType.cpp models/events/EventType.h
        models/aircraft/Engine.h
        models/settings/range.h
        models/SimConnect.h
        models/settings/range.h
        models/library/platform.cpp models/library/platform.h
        models/outputs/activeoutputs.cpp models/outputs/activeoutputs.h
        models/outputs/output.cpp models/outputs/output.h
        models/outputs/outputbundle.cpp models/outputs/outputbundle.h
        models/outputs/set.cpp models/outputs/set.h
        models/settings/coordinates.cpp models/settings/coordinates.h
        models/settings/settingsranges.cpp models/settings/settingsranges.h
        models/aircraft/Engine.cpp
        models/settings/range.cpp

        widgets/eventeditor/eventwindow.cpp widgets/eventeditor/eventwindow.h widgets/eventeditor/eventwindow.ui
        widgets/codegenerator/CodeGeneratorWindow.cpp widgets/codegenerator/CodeGeneratorWindow.h
        widgets/codegenerator/boards/Board.cpp widgets/codegenerator/boards/Board.h
        widgets/codegenerator/components/Part.cpp widgets/codegenerator/components/Part.h
        widgets/codegenerator/components/Pushbutton.cpp widgets/codegenerator/components/Pushbutton.h
        widgets/mainwindow/mainwindow.h
        widgets/librarygenerator/librarygenerator.cpp widgets/librarygenerator/librarygenerator.h
        widgets/librarygenerator/librarygeneratorwindow.cpp widgets/librarygenerator/librarygeneratorwindow.h widgets/librarygenerator/librarygeneratorwindow.ui
        widgets/outputmenu/outputmapper.cpp widgets/outputmenu/outputmapper.h
        widgets/axismenu/calibrateaxismenu.cpp widgets/axismenu/calibrateaxismenu.h widgets/axismenu/calibrateaxismenu.ui
        widgets/settingsmenu/builder/formbuilder.cpp widgets/settingsmenu/builder/formbuilder.h
        widgets/settingsmenu/optionsmenu.cpp widgets/settingsmenu/optionsmenu.h widgets/settingsmenu/optionsmenu.ui
        widgets/outputmenu/outputmenu.cpp widgets/outputmenu/outputmenu.h widgets/outputmenu/outputmenu.ui
        widgets/mainwindow/mainwindow.cpp widgets/mainwindow/mainwindow.ui

        handlers/pathhandler.cpp handlers/pathhandler.h
        handlers/outputhandler.cpp handlers/outputhandler.h
        handlers/settingshandler.cpp handlers/settingshandler.h

        serial/SerialPort.hpp serial/SerialPort.cpp

        rungaurd/RunGuard.cpp rungaurd/RunGuard.h

        logging/Logger.cpp logging/Logger.h
        logging/MessageCaster.cpp logging/MessageCaster.h
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/BitsAndDroidsConnector/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

set(Resources_resource_files
        "BitsAndDroidsLogo.ico"
        "resources/images/BitsAndDroidsLogo.ico"
        "widgets/codegenerator/boards/boardimages/proMicroPinout.png"
        "resources/fonts/DSEG7Classic-Bold.ttf"
        "resources/fonts/DSEG7Classic-Regular.ttf"
        "resources/fonts/Roboto-Black.ttf"
        "resources/fonts/Roboto-Bold.ttf"
        "resources/fonts/Roboto-Regular.ttf"
        "resources/images/SwitchArrow.png"
        "resources/images/avionicsSwitchOff.png"
        "resources/images/avionicsSwitchOn.png"
        "resources/images/batterySwitch.png"
        "resources/images/batterySwitchOff.png"
        "resources/images/refreshicon.png"
        "resources/images/starterImage.png"
        "resources/images/starterImageOff.png"
        "resources/images/toggleDown.png"
        "resources/images/toggleUp.png"
        )

qt_add_executable(
        BitsAndDroidsConnector WIN32
        ${PROJECT_SOURCES}
        ${APP_ICON_RESOURCE_WINDOWS}
)

target_link_libraries(BitsAndDroidsConnector PRIVATE
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::SerialPort
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Widgets
        "${PROJECT_SOURCE_DIR}/dist/SimConnect.dll"
        "${PROJECT_SOURCE_DIR}/dist/libssl-1_1-x64.dll"
        )

qt6_add_resources(BitsAndDroidsConnector "Resources"
        PREFIX
        "/"
        FILES
        ${Resources_resource_files}
        )

if (win64 AND NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(BitsAndDroidsConnector PUBLIC
            "${PROJECT_SOURCE_DIR}/dist/SimConnect.dll"
            )
endif ()

configure_file(BitsAndDroidsLogo.ico BitsAndDroidsConnector/bin/BitsAndDroidsLogo.ico COPYONLY)

add_custom_command(TARGET BitsAndDroidsConnector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dist" $<TARGET_FILE_DIR:BitsAndDroidsConnector>)
add_custom_command(TARGET BitsAndDroidsConnector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dist/platforms" "$<TARGET_FILE_DIR:BitsAndDroidsConnector>/platforms")
add_custom_command(TARGET BitsAndDroidsConnector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dist/styles" "$<TARGET_FILE_DIR:BitsAndDroidsConnector>/styles")
add_custom_command(TARGET BitsAndDroidsConnector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dist/iconengines" "$<TARGET_FILE_DIR:BitsAndDroidsConnector>/iconengines")
add_custom_command(TARGET BitsAndDroidsConnector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dist/imageformats" "$<TARGET_FILE_DIR:BitsAndDroidsConnector>/imageformats")

