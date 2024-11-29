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
        
        stage('Test Docker') {
            steps {
                script {
                    sh """
                        docker info
                        docker ps
                    """
                }
            }
        }
        
        stage('Deploy') {
            steps {
                script {
                    sh """
                        docker volume create coman-data || true
                        
                        # Stop and remove existing container if it exists
                        docker rm -f coman || true
                        
                        # Run new container with host networking
                        docker run -d \
                            --name coman \
                            --network jenkins \
                            -v coman-data:/data \
                            --restart unless-stopped \
                            ${DOCKER_IMAGE}:${VERSION}
                        
                        # Wait a moment for the container to start
                        sleep 5
                        
                        # Network diagnostics
                        docker exec coman netstat -tulpn || true
                        docker exec coman ss -tulpn || true
                        
                        # Test connectivity from inside container
                        docker exec coman curl -v http://localhost:18080 || true
                        docker exec coman curl -v http://127.0.0.1:18080 || true

                        # Test local connectivity
                        curl -v http://localhost:18080 || true
                        
                        # Verify container is running and check logs
                        echo "Container Status:"
                        docker ps -a | grep coman
                        
                        echo "Container Logs:"
                        docker logs coman
                        
                        docker logs coman

                        docker exec coman ip addr show || true
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