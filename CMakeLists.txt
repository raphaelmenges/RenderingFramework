### SETUP ###
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CONFIGURATION_TYPES Debug Release)
project(RenderingFramework)

# Configuration
set(CMAKE_CONFIGURATION_TYPES Debug;Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Pathes
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE INTERNAL "Project specific path. Set manually if it was not found.")
set(EXTERNALS_PATH ${CMAKE_SOURCE_DIR}/externals CACHE PATH "Project specific path. Set manually if it was not found.")
set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources CACHE PATH "Project specific path. Set manually if it was not found.")

# Defines for C++ side
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")

# Outputs

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exe CACHE PATH "Output path for executables.")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Output path for libraries.")

# Macros
include(${CMAKE_MODULE_PATH}/macros.cmake)

# Include project folder as include
include_directories(${CMAKE_SOURCE_DIR})

### COLLECT DEPENDENCIES ###
message(Collect Dependencies:)

# GLFW
message(GLFW)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "GLFW build examples." )
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "GLFW build tests.")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "GLFW build docs.")
set(GLFW_INSTALL OFF CACHE INTERNAL "GLFW install.")
set(GLFW_DOCUMENT_INTERNALS OFF CACHE INTERNAL "GLFW document internals.")
set(GLFW_USE_EGL OFF CACHE INTERNAL "GLFW use EGL.")
set(GLFW_USE_HYBRID_HPG OFF CACHE INTERNAL "GLFW use hybrid HPG.")
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE INTERNAL "MSCV runtime library dll.")
set(LIB_SUFFIX "" CACHE INTERNAL "Suffix of lib.")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "GLFW build shared libs.")
add_subdirectory(${EXTERNALS_PATH}/glfw)

# GLM (do not add as subdirectory)
message(GLM)
include_directories(${EXTERNALS_PATH}/glm)

# STB TODO
message(stb)

# gl3w
message(gl3w)
include_directories(${EXTERNALS_PATH}/gl3w/include)
file(GLOB GL3W ${EXTERNALS_PATH}/gl3w/src/*.c)

# OpenGL (should be found by CMake)
message(OpenGL)
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
