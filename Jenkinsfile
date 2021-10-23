node {
  stage 'Checkout code' 
  checkout scm
  stage'Build'
  sh "md -p output"
  sh 'qmake Bitsanddroidsgui.pro'
  sh 'make'
     }
