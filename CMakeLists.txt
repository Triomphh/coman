cmake_minimum_required(VERSION 3.10)
project(coman)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Use embedded SQLite3 library for ease of use
set(SQLITECPP_INTERNAL_SQLITE ON CACHE BOOL "Use internal SQLite3" FORCE)

# Add libraries
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/SQLiteCpp)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party/SQLiteCpp/include
)

# Source files (excluding main.cpp)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Main source file
set(MAIN_SOURCE "${PROJECT_SOURCE_DIR}/src/main.cpp")

# Main executable
add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    pthread
    SQLiteCpp
    dl
)

# Enable testing
enable_testing()

# Find GTest package
find_package(GTest REQUIRED)

# Test files
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

# Test executable
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES} ${SOURCES})
target_link_libraries(${PROJECT_NAME}_test
    GTest::GTest
    GTest::Main
    pthread
    SQLiteCpp
    dl
)
