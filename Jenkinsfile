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
                bat "Bitsanddroidsgui.pro"
                bat "make"
            }
        }
    }
}
