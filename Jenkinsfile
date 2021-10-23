pipeline{
  agent any
  stages {
    stage ('Checkout code'){
      steps{
        checkout scm      
      }
    }
    stage ('Build) {
        md -p output
        qmake Bitsanddroidsgui.pro
        make
           }
     }
          
  
}
