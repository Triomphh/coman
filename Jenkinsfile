pipeline {
    agent any

    environment {
        DOCKER_IMAGE = 'triomph/coman'
        VERSION = ''
        CONTAINER_NAME = 'coman-prod'
        VOLUME_NAME = 'coman-data'
        NETWORK_NAME = 'coman-network'
        VIRTUAL_HOST = 'coman.triomph.dev'
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
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
                    // Run tests inside a temporary container
                    sh """
                        docker run --rm ${DOCKER_IMAGE}:${VERSION} ./build/coman_tests
                    """
                }
            }
        }

        stage('Deploy') {
            steps {
                script {
                    // Create network if it doesn't exist
                    sh """
                        docker network create ${NETWORK_NAME} || true
                    """
                    
                    // Create volume if it doesn't exist
                    sh """
                        docker volume create ${VOLUME_NAME} || true
                    """
                    
                    // Stop and remove existing container if it exists
                    sh """
                        docker stop ${CONTAINER_NAME} || true
                        docker rm ${CONTAINER_NAME} || true
                    """
                    
                    // Run new container
                    sh """
                        docker run -d \
                            --name ${CONTAINER_NAME} \
                            --network ${NETWORK_NAME} \
                            -v ${VOLUME_NAME}:/app \
                            -e VIRTUAL_HOST=${VIRTUAL_HOST} \
                            -p 18080:18080 \
                            --restart unless-stopped \
                            ${DOCKER_IMAGE}:${VERSION}
                    """
                }
            }
        }
    }

    post {
        failure {
            // Cleanup on failure
            sh """
                docker stop ${CONTAINER_NAME} || true
                docker rm ${CONTAINER_NAME} || true
            """
        }
    }
}