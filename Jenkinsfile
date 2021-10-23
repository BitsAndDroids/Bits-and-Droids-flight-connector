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
                sh "md - p output"
                sh "qmake Bitsanddroidsgui.pro"
                sh "make"
            }
        }
    }
}
