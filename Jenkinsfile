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
                sh "md - p outputYup"
                sh "qmake Bitsanddroidsgui.pro"
                sh "make"
            }
        }
    }
}
