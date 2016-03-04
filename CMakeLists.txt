### SETUP ###
cmake_minimum_required(VERSION 2.8)
project(RenderingFramework)

# Configuration
set(CMAKE_CONFIGURATION_TYPES Debug;Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Pathes
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE PATH "Project specific path. Set manually if it was not found.")
set(EXTERNALS_PATH ${CMAKE_SOURCE_DIR}/externals CACHE PATH "Project specific path. Set manually if it was not found.")
set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources CACHE PATH "Project specific path. Set manually if it was not found.")

# Defines for C++ side
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")

# Outputs
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exe)

# Macros
include(${CMAKE_MODULE_PATH}/macros.cmake)

# Include project folder as include
include_directories(${CMAKE_SOURCE_DIR})

### COLLECT DEPENDENCIES ###
message(Collect Dependencies:)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW build examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW build tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW build docs")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW install")
add_subdirectory(${EXTERNALS_PATH}/glfw)

# GLM (do not add as subdirectory)
include_directories(${EXTERNALS_PATH}/glm)

# STB TODO

# gl3w
include_directories(${EXTERNALS_PATH}/gl3w/include)
file(GLOB GL3W ${EXTERNALS_PATH}/gl3w/src/*.c)

# OpenGL (should be found by CMake)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

### COLLECT UTILITIES ###
file(GLOB_RECURSE UTIL ${CMAKE_SOURCE_DIR}/src/util/*.cpp)

### GLOBAL CODE ###
set(GLOBAL_CODE ${UTIL} ${GL3W})

### COLLECT PROJECTS ###
message(Collect Projects:)

# Projects
SUBDIRLIST(SUBDIRS ${CMAKE_SOURCE_DIR}/src/exe)
foreach(SUBDIR ${SUBDIRS})
    message(${SUBDIR})
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/exe/${SUBDIR})
endforeach()
