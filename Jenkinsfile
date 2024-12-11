pipeline {
    agent any

    environment {
        DOCKER_IMAGE = 'triomph/coman'
        VERSION = sh(script: 'git rev-parse --short HEAD', returnStdout: true).trim()
        CONTAINER_NAME = 'coman'
        VOLUME_NAME = 'coman-data'
        APP_PORT = '18080'
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
                script {
                    // Run tests inside a temporary container
                    sh """
                        docker run --rm ${DOCKER_IMAGE}:${VERSION} /bin/bash -c './coman_test'
                    """
                }
            }
        }

        stage('Deploy') {
            steps {
                script {
                    // Create volume if it doesn't exist
                    sh "docker volume create ${VOLUME_NAME} || true"

                    // Stop and remove existing container if it exists
                    sh """
                        docker stop ${CONTAINER_NAME} || true
                        docker rm ${CONTAINER_NAME} || true
                    """

                    // Run the container
                    sh """
                        docker run -d --name ${CONTAINER_NAME} \
                            -v ${VOLUME_NAME}:/app/data \
                            -p ${APP_PORT}:${APP_PORT} \
                            --restart unless-stopped \
                            ${DOCKER_IMAGE}:${VERSION}
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
