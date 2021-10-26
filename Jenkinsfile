pipeline {
    agent any
    stages {
        stage ('Checkout code') {
            steps {
                checkout scm
                bat 'copy *.dll "release\"'       
            }
        }
        stage('Build') {
            steps {
                bat "C:/QtWindows/5.15.0/mingw81_64/bin/qmake.exe -r Bitsanddroidsgui.pro 'CONFIG+=release'"  
                bat "C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe clean"
                bat "C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe -j16"
            }
        }
    }
}
