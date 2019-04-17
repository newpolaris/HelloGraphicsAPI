# CMake entry point
cmake_minimum_required (VERSION 3.0)
project(HelloGraphicsAPI)

set(APP_TARGET HelloGraphicsAPI.app)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory(external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(UsebackportedModules)
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

# EXTENNAL
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

# GLAD
set(GLAD_DIR "${EXTERNAL_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")

# STB
set(STB_DIR "${EXTERNAL_DIR}/stb")
add_library("stb" "${STB_DIR}/stb_image.cpp")
target_include_directories("stb" PRIVATE "${STB_DIR}")

link_libraries(glfw)

include_directories(
	source/
	external/
	external/glm/
	${GLFW_INCLUDE_DIRS} 
	"${GLFW_SOURCE_DIR}/deps"
	.
)

set(ALL_LIBS
	"glad"
    "stb"
	${OPENGL_LIBRARY}
)

add_subdirectory(source)
