cmake_minimum_required( VERSION 3.7 )

# Project 
project(Str-EEA-t_Fighter_Server)

# Source & Header location
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

# Make the executable with the headers & sources
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Target the include folder (used to make headers visible in visual studio)
target_include_directories(${PROJECT_NAME} PRIVATE include)
