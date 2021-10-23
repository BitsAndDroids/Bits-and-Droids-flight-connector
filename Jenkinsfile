pipeline {
    agent any
    stages {
        stage ('Checkout code') {
            steps {
                checkout scm
                bat "copy SimConnect.dll /release"
                bat "copy Qt5Charts.dll /release"
                bat "copy Qt5Core.dll /release"
                bat "copy Qt5Gui.dll /release"
                bat "copy Qt5Network.dll /release"
                bat "copy Qt5PrintSupport.dll /release"
                 bat "copy Qt5SerialPort.dll /release"
                 bat "copy Qt5Svg.dll /release"
                 bat "copy Qt5Widgets.dll /release"
                bat "copy libssl-1_1-x64.dll /release"
                bat "copy libcrypto-1_1-x64.dll /release"
                
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
