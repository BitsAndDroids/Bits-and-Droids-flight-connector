pipeline {
    agent any
    stages {
        stage ('Checkout code') {
            steps {
                checkout scm
            }
        }
        stage('Build') {
            steps {
                bat "C:\QtWindows\5.15.0\mingw81_64\bin\qmake.exe -spec win32-g++ && C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe qmake_all"
                bat "mingw32-make.exe"
            }
        }
    }
}
