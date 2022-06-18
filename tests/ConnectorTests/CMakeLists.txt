cmake_minimum_required(VERSION 3.5)

project(OutputTests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OutputTests
        tst_outputtests.cpp
        ${CMAKE_SOURCE_DIR}/utils/OutputConverters.cpp ${CMAKE_SOURCE_DIR}/utils/OutputConverters.h)
add_test(NAME OutputTests COMMAND OutputTests)

target_link_libraries(OutputTests PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test)

