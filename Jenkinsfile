pipeline {
    agent any
    stages {
        stage ('Checkout code') {
            steps {
                checkout scm
                bat "move Simconnect.dll /release"
                bat "move Qt5Charts.dll /release"
                bat "move Qt5Core.dll /release"
                bat "move Qt5Gui.dll /release"
                bat "move Qt5Network.dll /release"
                bat "move Qt5PrintSupport.dll /release"
                 bat "move Qt5SerialPort.dll /release"
                 bat "move Qt5Svg.dll /release"
                 bat "move Qt5Widgets.dll /release"
                bat "move libssl-1_1-x64.dll /release"
                bat "move libcrypto-1_1-x64.dll /release"
                
            }
        }
        stage('Build') {
            steps {
                bat "C:/QtWindows/Tools/mingw81_64/bin/qmake.exe -r -spec win32-g++"
                bat "C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe qmake_all"
                bat "C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe -j16 in /release"
            }
        }
    }
}
