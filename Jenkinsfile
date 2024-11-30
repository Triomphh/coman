pipeline {
    agent any

    environment {
        DOCKER_IMAGE = 'triomph/coman'
        VERSION = ''
        CONTAINER_NAME = 'coman'
        VOLUME_NAME = 'coman-data'
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
                            -v ${VOLUME_NAME}:/app \
                            -p 18080:18080 \
                            ${DOCKER_IMAGE}:latest
                    """
                }
            }
        }
    }

    post {
        failure {
            sh """
                docker stop coman || true
                docker rm -f coman || true
            """
        }
    }
}
