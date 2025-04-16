# COMAN - Documentation

Modern and advanced documentation is available at [coman.triomph.dev/doc](https://coman.triomph.dev/doc)

## About
COMAN is a modern and efficient project management software, accessible at [coman.triomph.dev](https://coman.triomph.dev). This application follows a robust client-server architecture, designed to provide a smooth and high-performance user experience.

## Technical Architecture

### Backend
- Developed in C++ (version 17)
- REST API using the Crow framework
- Persistent database
- Layered architecture (MVC)
  - Models: Data representation
  - Controllers: HTTP request handling
  - Services: Business logic
  - Repositories: Data access

### Frontend
- User interface in HTML/CSS
- CSS Framework: Tailwind CSS
- Responsive and modern design


## Installation Prerequisites

### Local Installation
#### Required Dependencies
- CMake
- C++ 17 or higher
- Asio development headers (version 1.10.9+)

### Installation via Docker
Docker greatly simplifies the deployment of the application.

#### Docker Prerequisites
- Docker installed on your system
- Administrator rights for some commands

## Installation Guide

### Method 1: Installation with Docker (Recommended)

1. **Create the volume for the database**
   ```bash
   docker volume create coman-data
   ```

2. **Build the image**
   ```bash
   docker build -t triomph/coman:latest .
   ```

3. **Run the application**
   ```bash
   docker run -p 18080:18080 -v coman-data:/app/data triomph/coman:latest
   ```

Note: Depending on your configuration, you may need to use `sudo` for Docker commands.

#### Tests
To run the tests with Docker:
```bash
docker run --rm triomph/coman:latest /bin/bash -c './coman_test'
```

### Method 2: Local Installation

1. **Compilation**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

2. **Execution**
   ```bash
   ./coman
   ```

Important: Execution must be done from the root of the project.

## Version Management

The project follows the semantic versioning convention (SemVer):
- **MAJOR.MINOR.PATCH** (example: 1.18.2)
  - MAJOR (1.0.0): Breaking changes
  - MINOR (0.1.0): Backward-compatible feature additions
  - PATCH (0.0.1): Bug fixes

## Production Deployment

The project is automatically deployed on a VPS (Virtual Private Server) thanks to:
- **Jenkins** for continuous integration
- **GitHub Webhooks** that automatically trigger deployment on each push to the main branch
- Automated deployment pipeline including:
  - Code compilation
  - Test execution
  - Deployment to the production server

This setup enables continuous and secure deployment without manual intervention.

## Maintenance and Support


### Troubleshooting Common Issues
1. **Initial compilation errors**: Check comments in `CMakeLists.txt`
2. **Docker permission issues**: Use `sudo` if necessary
3. **Path errors**: Make sure you are in the correct directory for execution
4. **Site inaccessible**: Try another network (e.g. 5G, 4G)

