# Generated from Bitsanddroidsgui.pro.

cmake_minimum_required(VERSION 3.16)
project(BitsAndDroidsConnector VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Gui Network PrintSupport SerialPort svg REQUIRED)

set(PROJECT_SOURCES

        main.cpp
        constants.h

        handlers/InputSwitchHandler.cpp
        handlers/InputSwitchHandler.h
        outputmenu/handlers/sethandler.cpp outputmenu/handlers/sethandler.h

        dashboard/workers/MFSWorker.cpp dashboard/workers/MFSWorker.h
        workers/InputMapper.h workers/inputmapper.cpp
        services/ServiceWorker.cpp services/ServiceWorker.h

        services/LoggerService.cpp services/LoggerService.h

        stores/LoggerStore.cpp stores/LoggerStore.h

        elements/ModeIndexCheckbox.cpp elements/ModeIndexCheckbox.h
        elements/ModeIndexCombobox.cpp elements/ModeIndexCombobox.h
        elements/mcheckbox.cpp elements/mcheckbox.h

        enums/ModeEnum.h
        enums/InputTypeEnum.h
        enums/AxisTypeEnum.h
        enums/inputenum.cpp enums/inputenum.h
        enums/outputenum.cpp enums/outputEnum.h
        enums/CurveTypeEnum.h
        enums/LogLevelEnum.h

        models/events/Event.h
        models/events/EventDatatype.cpp models/events/EventDatatype.h
        models/events/EventType.cpp models/events/EventType.h
        models/aircraft/Engine.h
        models/settings/range.h
        models/SimConnect.h
        models/settings/range.h
        models/library/platform.cpp models/library/platform.h
        models/commands/activeoutputs.cpp models/commands/activeoutputs.h
        models/commands/output.cpp models/commands/output.h
        models/commands/outputbundle.cpp models/commands/outputbundle.h
        models/commands/Set.cpp models/commands/Set.h
        models/settings/coordinates.cpp models/settings/coordinates.h
        models/settings/settingsranges.cpp models/settings/settingsranges.h
        models/aircraft/Engine.cpp
        models/settings/range.cpp
        models/aircraft/Axis.cpp models/aircraft/Axis.h
        models/aircraft/CurveAxis.cpp models/aircraft/CurveAxis.h
        models/Log.cpp models/Log.h

        widgets/eventeditor/eventwindow.cpp widgets/eventeditor/eventwindow.h widgets/eventeditor/eventwindow.ui
        widgets/codegenerator/CodeGeneratorWindow.cpp widgets/codegenerator/CodeGeneratorWindow.h
        widgets/codegenerator/boards/Board.cpp widgets/codegenerator/boards/Board.h
        widgets/codegenerator/components/Part.cpp widgets/codegenerator/components/Part.h
        widgets/codegenerator/components/Pushbutton.cpp widgets/codegenerator/components/Pushbutton.h
        dashboard/Dashboard.h
        widgets/librarygenerator/librarygenerator.cpp widgets/librarygenerator/librarygenerator.h
        widgets/librarygenerator/librarygeneratorwindow.cpp widgets/librarygenerator/librarygeneratorwindow.h widgets/librarygenerator/librarygeneratorwindow.ui
        outputmenu/outputmapper.cpp outputmenu/outputmapper.h
        widgets/axismenu/calibrateaxismenu.cpp widgets/axismenu/calibrateaxismenu.h widgets/axismenu/calibrateaxismenu.ui
        widgets/settingsmenu/builder/formbuilder.cpp widgets/settingsmenu/builder/formbuilder.h
        widgets/settingsmenu/optionsmenu.cpp widgets/settingsmenu/optionsmenu.h widgets/settingsmenu/optionsmenu.ui
        outputmenu/outputmenu.cpp outputmenu/outputmenu.h
        dashboard/Dashboard.cpp

        handlers/pathhandler.cpp handlers/pathhandler.h
        handlers/outputhandler.cpp handlers/outputhandler.h
        settings/settingshandler.cpp settings/settingshandler.h
        services/InstallationService.cpp services/InstallationService.h
        serial/SerialPort.hpp serial/SerialPort.cpp

        rungaurd/RunGuard.cpp rungaurd/RunGuard.h
        dashboard/models/ComBundle.h dashboard/models/ComBundle.cpp
        logging/Logger.cpp logging/Logger.h
        logging/LogWindow.cpp logging/LogWindow.h
        logging/MessageCaster.cpp logging/MessageCaster.h
        utils/OutputConverters.cpp utils/OutputConverters.h
        enums/OutputModesEnum.h models/commands/Input.cpp models/commands/Input.h utils/InputReader.cpp utils/InputReader.h
        outputmenu/builder/SetrowBuilder.cpp outputmenu/builder/SetrowBuilder.h
        outputmenu/builder/OutputMenuBuilder.cpp outputmenu/builder/OutputMenuBuilder.h
        outputmenu/elements/SetRow.cpp outputmenu/elements/SetRow.h
        elements/MPushButton.cpp elements/MPushButton.h
        outputmenu/elements/OutputTabs.cpp outputmenu/elements/OutputTabs.h
        outputmenu/builder/OutputTabBuilder.cpp outputmenu/builder/OutputTabBuilder.h
        outputmenu/elements/SetDetails.cpp outputmenu/elements/SetDetails.h
        elements/MStyleLabels.cpp elements/MStyleLabels.h enums/StyleEnum.h
        outputmenu/builder/OutputDetailsBuilder.cpp outputmenu/builder/OutputDetailsBuilder.h
        utils/JSONParser.cpp utils/JSONParser.h
        utils/EventFileFormatter.cpp utils/EventFileFormatter.h
        dashboard/models/ComPortWidgetModel.cpp dashboard/models/ComPortWidgetModel.h
        dashboard/elements/ComPortRow.cpp dashboard/elements/ComPortRow.h
        dashboard/controller/ComPortWidgetController.cpp dashboard/controller/ComPortWidgetController.h
        elements/Element.cpp elements/Element.h
        dashboard/Elements/ComPortWidget.cpp dashboard/Elements/ComPortWidget.h
        dashboard/elements/ComPortButtonRow.cpp dashboard/elements/ComPortButtonRow.h
        dashboard/elements/MenuBar.cpp dashboard/elements/MenuBar.h dashboard/controller/DashboardController.cpp dashboard/controller/DashboardController.h dashboard/handlers/WASMHandler.cpp dashboard/handlers/WASMHandler.h settings/migrations/Migration.h settings/migrations/2022092601.h settings/ComSettingsHandler.cpp settings/ComSettingsHandler.h settings/ServiceSettingsHandler.cpp settings/ServiceSettingsHandler.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/BitsAndDroidsConnector/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc" )

set(Resources_resource_files
        "BitsAndDroidsLogo.ico"
        "resources/images/BitsAndDroidsLogo.ico"
        "widgets/codegenerator/boards/boardimages/proMicroPinout.png"
        "resources/fonts/DSEG7Classic-Bold.ttf"
        "resources/fonts/DSEG7Classic-Regular.ttf"
        "resources/fonts/Roboto-Black.ttf"
        "resources/fonts/Roboto-Bold.ttf"
        "resources/fonts/Roboto-Regular.ttf"
        "resources/images/edit.svg"
        "resources/images/trashcan.svg"
        "resources/images/export.svg"
        "resources/images/save.svg"
        "resources/images/reload.svg"
        "resources/images/arrow.png"
        "resources/images/add.svg"
        )

qt_add_executable(
        BitsAndDroidsConnector WIN32
        ${PROJECT_SOURCES}
        ${APP_ICON_RESOURCE_WINDOWS}

)
set_target_properties (BitsAndDroidsConnector PROPERTIES DEBUG_POSTFIX "-Insider")
target_link_libraries(BitsAndDroidsConnector PRIVATE
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::SerialPort
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

if (win64 AND NOT CMAKE_BUILD_TYPE STREQUAL DEBUG)
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


#-----------------------------------------------------#
#                   TESTING SECTION                   #
#-----------------------------------------------------#


IF(CMAKE_BUILD_TYPE MATCHES Debug)
    include(CTest)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
            INSTALL_COMMAND ""
    )
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
    include(googletest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()

    add_executable(UnitTests
            utils/tests/UtilTests.cpp
            outputmenu/tests/SethandlerTests.cpp
            models/commands/Set.cpp
            utils/OutputConverters.cpp
            utils/tests/UtilTests.cpp
            settings/settingshandler.cpp
            handlers/outputhandler.cpp
            handlers/pathhandler.cpp
            models/commands/output.cpp
            logging/messagecaster.cpp
            outputmenu/handlers/SetHandler.cpp
            settings/ComSettingsHandler.cpp
            utils/EventFileFormatter.cpp settings/tests/ComSettingsHandlerTests.cpp)


    add_dependencies(UnitTests gtest)

    target_link_libraries(UnitTests
            GTest::gtest_main
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Widgets)

    gtest_discover_tests(
            UnitTests
    )
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)