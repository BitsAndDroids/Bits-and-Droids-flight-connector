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
                md - p output
                qmake Bitsanddroidsgui.pro
                make
            }
        }
    }
}
