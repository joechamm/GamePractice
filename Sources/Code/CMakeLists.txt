cmake_minimum_required(VERSION 3.15)

set(MAIN_PROJECT_NAME "Game01" CACHE INTERNAL "Build target name of main project")
set(MAIN_PROJECT_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to sources of main project")

project(${MAIN_PROJECT_NAME} CXX)

# Detect & verify project configuration

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed for this project")
else()
    message(STATUS "Binaries output directory: ${CMAKE_BINARY_DIR}")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/compile_flags.cmake)

# Build target project
set(APPLICATION_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Application)

add_subdirectory(Engine)
add_subdirectory(Application)
add_subdirectory(Editor)

if(NOT BUILD_PLATFORM STREQUAL "Android")
    message(STATUS "Add unittest to build results")
    add_subdirectory(Tests)
else()
    message(STATUS "Android platform does not support unittests")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_PLATFORM STREQUAL "Android")
    add_library(${PROJECT_NAME} SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/android_main.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/android_main.cpp)
else()
    add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/desktop_main.cpp)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_PDB_NAME ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Engine Application)