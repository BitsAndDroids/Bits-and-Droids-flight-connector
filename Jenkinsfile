node {
  stage 'Checkout code' 
  checkout scm
  stage'Build'
  sh "mkd -p output"
  sh 'qmake'
  sh 'make'
     }
