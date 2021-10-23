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
                bat "-spec win32-g++ && C:/QtWindows/Tools/mingw810_64/bin/mingw32-make.exe qmake_all"
                bat "mingw32-make.exe"
            }
        }
    }
}
