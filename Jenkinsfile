pipeline {
    agent any

    stages {
        stage('Checkout') {
            steps {
                git(url: 'https://github.com/Triomphh/coman', branch: 'main')
            }
        }

        stage('Build') {
            steps {
                sh 'echo "Building..."'
            }
        }

        stage('Test') {
            steps {
                sh 'echo "Testing..."'
            }
        }

        stage('Deploy') {
            steps {
                sh 'echo "Deploying..."'
            }
        }
    }
}