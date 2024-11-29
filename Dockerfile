FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsqlite3-dev \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . .

# Create build directory and build the project
RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

# Make sure the binary is executable
RUN chmod +x /app/coman

# Create a directory for the database with proper permissions
RUN mkdir -p /data && \
    chmod 777 /data

# Expose the port that the application listens on
EXPOSE 18080

# Set the working directory to where the database should be
WORKDIR /data

# Run the application
CMD ["/app/coman"]