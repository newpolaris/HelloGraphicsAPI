# CMake entry point
cmake_minimum_required (VERSION 3.0)
project(HelloGraphicsAPI)

set(ROOT_PATH "${PROJECT_SOURCE_DIR}")
set(DEPENDENCIES_PATH "${PROJECT_SOURCE_DIR}/external")
set(SAMPLE_PATH "${PROJECT_SOURCE_DIR}/sample")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(UsebackportedModules)
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

include_directories(${PROJECT_SOURCE_DIR}/source)
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/deps)
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/external/meshoptimizer/src)
include_directories(${PROJECT_SOURCE_DIR}/external/objparser)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(source)
add_subdirectory(sample)
