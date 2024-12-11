FROM ubuntu:24.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsqlite3-dev \
    libasio-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Create data directory
RUN mkdir -p /app/data

# Copy the current directory contents into the container at /app
COPY . .

# Create build directory and build the project
RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

# Expose the port that the application listens on
EXPOSE 18080

# Run the application
CMD ["./coman"]