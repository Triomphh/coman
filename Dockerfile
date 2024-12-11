FROM ubuntu:24.04 AS builder

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

# Copy the current directory contents into the container at /app
COPY . .

# Create build directory and build the project
RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

# Test stage
FROM builder AS test
RUN /app/coman_test

# Production stage
FROM ubuntu:24.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libsqlite3-0 \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Create data directory
RUN mkdir -p /app/data

# Copy only the necessary files from builder
COPY --from=builder /app/coman .
COPY --from=builder /app/data /app/data

# Expose the port that the application listens on
EXPOSE 18080

# Run the application
CMD ["./coman"]