cmake_minimum_required(VERSION 3.10)
project(coman)


# Set C++ standard
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)



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




# Source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    pthread
    SQLiteCpp
    dl
)
