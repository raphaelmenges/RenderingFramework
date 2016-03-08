### SETUP ###
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CONFIGURATION_TYPES Debug Release)
project(RenderingFramework)

# Configuration
if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
endif(NOT MSVC)

# Pathes
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE INTERNAL "Project specific path. Set manually if it was not found.")
set(EXTERNALS_PATH ${CMAKE_SOURCE_DIR}/externals CACHE PATH "Project specific path. Set manually if it was not found.")
set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources CACHE PATH "Project specific path. Set manually if it was not found.")
set(SHADERS_PATH ${CMAKE_SOURCE_DIR}/shaders CACHE PATH "Project specific path. Set manually if it was not found.")

# Defines for C++ side
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")

# Outputs
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exe CACHE PATH "Output path for executables.")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Output path for libraries.")

# Macros
include(${CMAKE_MODULE_PATH}/macros.cmake)

# Include project folder as include
include_directories(${CMAKE_SOURCE_DIR})

### COLLECT DEPENDENCIES ###
message(STATUS ***Collect_Dependencies***)

# GLFW
message(STATUS GLFW)
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
message(STATUS GLM)
include_directories(${EXTERNALS_PATH}/glm)

# STB TODO
message(STATUS stb)

# ASSIMP TODO
message(STATUS assimp)

# gl3w
message(STATUS gl3w)
include_directories(${EXTERNALS_PATH}/gl3w/include)
file(GLOB GL3W ${EXTERNALS_PATH}/gl3w/src/*.c)

# ImGui
message(STATUS ImGui)
set(IM_GUI_DIR ${EXTERNALS_PATH}/imgui)
file(GLOB IM_GUI
    ${IM_GUI_DIR}/imgui.cpp
    ${IM_GUI_DIR}/imgui_draw.cpp
    ${IM_GUI_DIR}/examples/opengl3_example/imgui_impl_glfw_gl3.cpp)
include_directories(${IM_GUI_DIR})

# OpenGL (should be found by CMake)
message(STATUS OpenGL)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

### COLLECT UTILITIES ###
set(UTIL_PATH ${CMAKE_SOURCE_DIR}/src/util)
file(GLOB_RECURSE UTIL ${UTIL_PATH}/*.h ${UTIL_PATH}/*.cpp)

### GLOBAL CODE ###
set(GLOBAL_CODE ${UTIL} ${GL3W})

# Add shaders as subdirectory to display them in IDE
add_subdirectory(${SHADERS_PATH})

### COLLECT PROJECTS ###
message(STATUS ***Collect_Projects***)

# Projects
SUBDIRLIST(SUBDIRS ${CMAKE_SOURCE_DIR}/src/exe)
foreach(SUBDIR ${SUBDIRS})
    message(STATUS ${SUBDIR})
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/exe/${SUBDIR})
endforeach()
