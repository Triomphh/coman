pipeline {
    agent none

    environment {
        DOCKER_IMAGE = 'triomph/coman'
        VERSION = ''
        CONTAINER_NAME = 'coman-prod'
        VOLUME_NAME = 'coman-data'
        NETWORK_NAME = 'coman-network'
        VIRTUAL_HOST = 'coman.triomph.dev'
    }

    stages {
        stage('Build') {
            agent {
                docker {
                    image 'ubuntu:22.04'
                    args '-v $HOME/.cache:/root/.cache'
                    reuseNode true
                }
            }
            steps {
                // Install git first
                sh 'apt-get update && apt-get install -y git'
                
                checkout scm
                sh 'git submodule update --init --recursive'
                
                script {
                    // Get version
                    VERSION = sh(
                        script: 'git describe --tags --abbrev=0 2>/dev/null || echo "0.0.1"',
                        returnStdout: true
                    ).trim()
                    
                    // Build Docker image
                    docker.build("${DOCKER_IMAGE}:${VERSION}", "-t ${DOCKER_IMAGE}:latest .")
                }
            }
        }

        stage('Test') {
            agent any
            steps {
                echo "Running tests... (placeholder)"
                echo "Version: ${VERSION}"
                echo "Image: ${DOCKER_IMAGE}:${VERSION}"
            }
        }

        stage('Deploy') {
            agent any
            steps {
                script {
                    docker.withServer('unix:///var/run/docker.sock') {
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
                        docker.image("${DOCKER_IMAGE}:${VERSION}").withRun("""
                            --name ${CONTAINER_NAME} \
                            --network ${NETWORK_NAME} \
                            -v ${VOLUME_NAME}:/app \
                            -e VIRTUAL_HOST=${VIRTUAL_HOST} \
                            -p 18080:18080 \
                            --restart unless-stopped
                        """)
                    }
                }
            }
        }
    }

    post {
        failure {
            agent any
            steps {
                script {
                    // Cleanup on failure
                    sh """
                        docker stop ${CONTAINER_NAME} || true
                        docker rm ${CONTAINER_NAME} || true
                    """
                }
            }
        }
    }
}