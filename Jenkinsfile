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
                bat "C:/QtWindows/5.15.0/mingw81_64/bin/qmake.exe -spec win32-g++"
                bat "make"
            }
        }
    }
}
