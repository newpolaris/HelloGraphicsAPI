# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (HelloGraphicsAPI)

set(APP_TARGET HelloGraphicsAPI.app)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(UsebackportedModules)
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

include_directories(
	source/
	external/
	external/glm/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
)

file(GLOB_RECURSE SOURCE source/*)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(${APP_TARGET} ${SOURCE})
target_link_libraries(${APP_TARGET} ${ALL_LIBS})

# Xcode and Visual working directories
set_target_properties(${APP_TARGET} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
create_target_launcher(${APP_TARGET} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
