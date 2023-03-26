cmake_minimum_required( VERSION 3.8 )

# Project 
project(Str-EEA-t_Fighter)

# Source & Header location
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

# Look for OpenCV packages installed
find_package( OpenCV REQUIRED )

# Look for SDL2 packages installed
# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available 
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Look for SDL2_Image Packages
find_package(SDL2_image REQUIRED CONFIG)

# Look for SDL2_Mixer Packages
find_package(SDL2_mixer REQUIRED)

# Look for SDL2_TTF Packages
find_package(SDL2_ttf REQUIRED)

# Look for Lib realsense Packages
find_package(realsense2 REQUIRED CONFIG)

# Make the executable with the headers & sources
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Set the directory where CMakeLists.txt is to be the working directory
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Target the include folder (used to make headers visible in visual studio)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications  
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

# Link to the SDL2_ttf library
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)

# Link to the SDL2_image library
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)

# Link to the SDL2_mixer library
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer)

# Link to the realsense2 library
target_link_libraries(${PROJECT_NAME} PRIVATE ${realsense2_LIBRARY})

# Link OpenCV libraries
target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} )

# Copy the necessary DLLs to the executable folder (Thanks Stack Overflow)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
    )
