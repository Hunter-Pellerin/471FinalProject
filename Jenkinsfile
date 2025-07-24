pipeline {
    agent any
    stages {
        stage('Build') { 
            steps {
                sh 'rm -rf build'
                sh 'make -j$(nproc)' 
            }
        }
    }
}
