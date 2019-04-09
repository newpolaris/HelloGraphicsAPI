# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (HelloGraphicsAPI)

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

# GLAD
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(GLAD_DIR "${EXTERNAL_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")


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

set(OSX_SOURCE
    source/OSX/AppDelegate.mm
    source/OSX/GameViewController.mm
    source/OSX/main.mm
    source/OSX/AppDelegate.h
    source/OSX/GameViewController.h
    source/OSX/SharedStructures.h)

add_executable(
    ${APP_TARGET} 
    MACOSX_BUNDLE 
    ${SOURCE} 
    ${GLAD} 
    ${OSX_SOURCE} 
    source/OSX/MainMenu.xib 
    source/OSX/Shaders.metal)

set_source_files_properties(
    source/OSX/MainMenu.xib
    source/OSX/Shaders.metal
    PROPERTIES
    MACOSX_PACKAGE_LOCATION
    Resources)

set_target_properties(
    ${APP_TARGET}
    PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST
    ${CMAKE_CURRENT_LIST_DIR}/source/OSX/plist.in)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
  -framework AppKit \
  -framework Metal \
  -framework MetalKit \
  -framework ModelIO")

target_include_directories(${APP_TARGET} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${APP_TARGET} ${ALL_LIBS})

# Xcode and Visual working directories
if (APPLE)
	set_target_properties(${APP_TARGET} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
endif()
create_target_launcher(${APP_TARGET} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
