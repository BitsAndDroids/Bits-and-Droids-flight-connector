jenkinfsile

pipeline {
        agent any
        stages{
                stage('Checkout code') {
                steps {
                    checkout scm
                }
              }
                stage('Build'){
                        steps {
                                qmake Bitsanddroidsgui.pro -spec win32-g++
                        }
    }
        }
}
