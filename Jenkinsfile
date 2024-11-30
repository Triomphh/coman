pipeline {
    agent any

    environment {
        DOCKER_IMAGE = 'triomph/coman'
        VERSION = '0.0.1'
        CONTAINER_NAME = 'coman'
        VOLUME_NAME = 'coman-data'
        APP_PORT = '18080'
        DOCKER_NETWORK = 'jenkins'
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
                sh 'git submodule update --init --recursive'
            }
        }

        stage('Build') {
            steps {
                script {
                    sh "docker build -t ${DOCKER_IMAGE}:${VERSION} -t ${DOCKER_IMAGE}:latest ."
                }
            }
        }

        stage('Test') {
            steps {
                sh 'echo "Testing..."'
            }
        }

        stage('Deploy') {
            steps {
                script {
                    // Add debugging information
                    sh """
                        echo "Available networks:"
                        docker network ls
                        
                        echo "\nNetwork details:"
                        docker network inspect jenkins || true
                        
                        echo "\nCurrent user:"
                        id
                    """

                    // Create volume if it doesn't exist
                    sh "docker volume create ${VOLUME_NAME} || true"

                    // Stop and remove existing container if it exists
                    sh """
                        docker stop ${CONTAINER_NAME} || true
                        docker rm ${CONTAINER_NAME} || true
                    """

                    // Run the container with explicit network connect
                    sh """
                        docker run -d --name ${CONTAINER_NAME} \
                            -v ${VOLUME_NAME}:/app \
                            -p ${APP_PORT}:${APP_PORT} \
                            --restart unless-stopped \
                            ${DOCKER_IMAGE}:${VERSION} && \
                        docker network connect ${DOCKER_NETWORK} ${CONTAINER_NAME}
                    """
                }
            }
        }
    }

    post {
        failure {
            script {
                sh """
                    docker stop ${CONTAINER_NAME} || true
                    docker rm ${CONTAINER_NAME} || true
                """
            }
        }
    }
}
