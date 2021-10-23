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
                                QtWindows/5.15.0/mingw81_64/bin/qmake make && make check
                                mkdir BitsAndDroidsGui
                                qmake Bitsanddroidsgui.pro -spec win32-g++
                        }
    }
        }
}
