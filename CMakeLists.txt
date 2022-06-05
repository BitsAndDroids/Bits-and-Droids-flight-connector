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
    Inputs/InputMapper.h Inputs/inputmapper.cpp
    Inputs/InputSwitchHandler.cpp
    Inputs/InputSwitchHandler.h
    Inputs/InputWorker.cpp Inputs/InputWorker.h
    Inputs/inputenum.cpp Inputs/inputenum.h
    codegenerator/CodeGeneratorWindow.cpp codegenerator/CodeGeneratorWindow.h
    codegenerator/boards/Board.cpp codegenerator/boards/Board.h
    codegenerator/components/Part.cpp codegenerator/components/Part.h
    codegenerator/components/Pushbutton.cpp codegenerator/components/Pushbutton.h
    dual/dualworker.cpp dual/dualworker.h
    elements/ModeIndexCheckbox.cpp elements/ModeIndexCheckbox.h
    elements/ModeIndexCombobox.cpp elements/ModeIndexCombobox.h
    elements/mcheckbox.cpp elements/mcheckbox.h
    enums/ModeEnum.h
    events/Event.h
    events/EventDataType.cpp events/EventDataType.h
    events/EventType.cpp events/EventType.h
    events/eventwindow.cpp events/eventwindow.h events/eventwindow.ui
    handlers/pathhandler.cpp handlers/pathhandler.h
    headers/Engine.h
    headers/SerialPort.hpp
    headers/SimConnect.h
    headers/constants.h
    headers/mainwindow.h
    headers/range.h
    library/librarygenerator.cpp library/librarygenerator.h
    library/librarygeneratorwindow.cpp library/librarygeneratorwindow.h library/librarygeneratorwindow.ui
    library/platform.cpp library/platform.h
    logging/Logger.cpp logging/Logger.h
    logging/MessageCaster.cpp logging/MessageCaster.h
    outputs/activeoutputs.cpp outputs/activeoutputs.h
    outputs/output.cpp outputs/output.h
    outputs/outputbundle.cpp outputs/outputbundle.h
    outputs/outputenum.cpp outputs/outputenum.h
    outputs/outputhandler.cpp outputs/outputhandler.h
    outputs/outputmapper.cpp outputs/outputmapper.h
    outputs/outputworker.cpp outputs/outputworker.h
    outputs/set.cpp outputs/set.h
    outputs/sethandler.cpp outputs/sethandler.h
    rungaurd/RunGuard.cpp rungaurd/RunGuard.h
    settings/calibrateaxismenu.cpp settings/calibrateaxismenu.h settings/calibrateaxismenu.ui
    settings/coordinates.cpp settings/coordinates.h
    settings/formBuilder.cpp settings/formBuilder.h
    settings/optionsmenu.cpp settings/optionsmenu.h settings/optionsmenu.ui
    settings/outputmenu.cpp settings/outputmenu.h settings/outputmenu.ui
    settings/settingshandler.cpp settings/settingshandler.h
    settings/settingsranges.cpp settings/settingsranges.h
    sources/Engine.cpp
    sources/SerialPort.cpp
    sources/main.cpp
    sources/mainwindow.cpp sources/mainwindow.ui
    sources/range.cpp
   )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/BitsAndDroidsConnector/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

set(Resources_resource_files
    "BitsAndDroidsLogo.ico"
    "images/BitsAndDroidsLogo.ico"
    "codegenerator/boards/boardimages/proMicroPinout.png"
    "fonts/DSEG7Classic-Bold.ttf"
    "fonts/DSEG7Classic-Regular.ttf"
    "fonts/Roboto-Black.ttf"
    "fonts/Roboto-Bold.ttf"
    "fonts/Roboto-Regular.ttf"
    "images/SwitchArrow.png"
    "images/avionicsSwitchOff.png"
    "images/avionicsSwitchOn.png"
    "images/batterySwitch.png"
    "images/batterySwitchOff.png"
    "images/refreshicon.png"
    "images/starterImage.png"
    "images/starterImageOff.png"
    "images/toggleDown.png"
    "images/toggleUp.png"
    "json/outputs.json"

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

if(win64 AND NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(BitsAndDroidsConnector PUBLIC
        "${PROJECT_SOURCE_DIR}/dist/SimConnect.dll"
    )
endif()

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

