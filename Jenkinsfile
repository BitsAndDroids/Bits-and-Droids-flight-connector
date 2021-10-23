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
                                mkdir BitsAndDroidsGui
                                qmake Bitsanddroidsgui.pro -spec win32-g++
                        }
    }
        }
}
