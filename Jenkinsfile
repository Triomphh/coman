pipeline {
    agent any
    
    environment {
        DOCKER_IMAGE = 'coman'
        VERSION = ''
    }
    
    stages {
        stage('Checkout') {
            steps {
                checkout scm
                // Initialize and update git submodules
                sh 'git submodule update --init --recursive'
            }
        }
        
        stage('Get Version') {
            steps {
                script {
                    // Read version from git tags, or set to 0.0.1 if no tags exist
                    VERSION = sh(
                        script: 'git describe --tags --abbrev=0 2>/dev/null || echo "0.0.1"',
                        returnStdout: true
                    ).trim()
                }
            }
        }
        
        stage('Build Docker Image') {
            steps {
                script {
                    // Build Docker image with version tag and latest tag
                    sh """
                        docker build -t ${DOCKER_IMAGE}:${VERSION} -t ${DOCKER_IMAGE}:latest .
                    """
                }
            }
        }
        
        stage('Test') {
            steps {
                script {
                    sh """
                        echo "Testing..."
                    """
                }
            }
        }
        
        stage('Deploy') {
            steps {
                script {
                    // Create volume if it doesn't exist
                    sh """
                        docker volume create coman-data || true
                    """
                    
                    // Stop and remove existing container if it exists
                    sh """
                        docker rm -f coman || true
                    """
                    
                    // Run new container
                    sh """
                        docker run -d \
                            --name coman \
                            -p 18080:18080 \
                            -v coman-data:/app \
                            --restart unless-stopped \
                            ${DOCKER_IMAGE}:${VERSION}
                    """
                }
            }
        }
    }
    
    post {
        always {
            cleanWs()
        }
        
        failure {
            echo 'Pipeline failed! Notify team...'
        }
    }
}