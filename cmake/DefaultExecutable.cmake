cmake_minimum_required(VERSION 2.8)

# Name the project by folder
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

# Code
file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Combine headers and sources with global code
set(CODE ${GLOBAL_CODE} ${SOURCES} ${HEADERS})

# Create executable
add_executable(${PROJECT_NAME} ${CODE})

# Link to executable
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw)