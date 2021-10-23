node {
  stage 'Checkout code' 
  checkout scm
  stage'Build'
  sh "mkdir -p output"
  sh 'qmake Bitsanddroidsgui.pro'
  sh 'make'
     }
