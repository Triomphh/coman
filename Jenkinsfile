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

        stage('Build and Test') {
            steps {
                script {
                    try {
                        // Build and run tests using the test stage
                        sh "docker build --target test -t ${DOCKER_IMAGE}:${VERSION}-test ."
                        currentBuild.result = 'SUCCESS'
                    } catch (Exception e) {
                        currentBuild.result = 'FAILURE'
                        error("Tests failed: ${e.message}")
                    }
                }
            }
        }

        stage('Build Production Image') {
            when {
                expression { currentBuild.result == 'SUCCESS' }
            }
            steps {
                script {
                    // Build the final production image
                    sh "docker build -t ${DOCKER_IMAGE}:${VERSION} -t ${DOCKER_IMAGE}:latest ."
                }
            }
        }

        stage('Deploy') {
            when {
                expression { currentBuild.result == 'SUCCESS' }
            }
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
        always {
            // Clean up images
            script {
                sh """
                    docker rmi ${DOCKER_IMAGE}:${VERSION}-test || true
                    docker rmi ${DOCKER_IMAGE}:${VERSION} || true
                    docker rmi ${DOCKER_IMAGE}:latest || true
                    docker image prune -f
                """
            }
        }
    }
}
