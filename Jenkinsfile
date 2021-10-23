pipeline {
    agent any
    stages {
        stage ('Checkout code') {
            steps {
                checkout scm
                bat "copy SimConnect.dll /release/SimConnect.dll"
                bat "copy Qt5Charts.dll /release/Qt5Charts.dll"
                bat "copy Qt5Core.dll /release/Qt5Core.dll"
                bat "copy Qt5Gui.dll /release/Qt5Gui.dll"
                bat "copy Qt5Network.dll /release/Qt5Network.dll"
                bat "copy Qt5PrintSupport.dll /release/Qt5PrintSupport.dll"
                 bat "copy Qt5SerialPort.dll /release/Qt5SerialPort.dll"
                 bat "copy Qt5Svg.dll /release/Qt5Svg.dll"
                 bat "copy Qt5Widgets.dll /release/Qt5Widgets.dll"
                bat "copy libssl-1_1-x64.dll /release/libssl-1_1-x64.dll"
                bat "copy libcrypto-1_1-x64.dll /release/libcrypto-1_1-x64.dll"
                
            }
        }
        stage('Build') {
            steps {
                bat "C:/QtWindows/5.15.0/mingw81_64/bin/qmake.exe -r -spec win32-g++"
                bat "C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe qmake_all"
                bat "C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe -j16 /release"
            }
        }
    }
}
