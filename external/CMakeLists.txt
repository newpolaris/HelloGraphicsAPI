if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()

include_directories(${glfw_INCLUDE_DIRS} "${GLFW_SOURCE_DIR}/deps")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(mtlpp)

set_target_properties(glfw PROPERTIES FOLDER "external")
set_target_properties(mtlpp PROPERTIES FOLDER "external")
set_target_properties(glad PROPERTIES FOLDER "external")

# stb
project(STB)
set(STB_DIR "${DEPENDENCIES_PATH}/stb")
add_library("stb" "${STB_DIR}/stb_image.cpp")
target_include_directories("stb" PRIVATE "${STB_DIR}")
set_target_properties("stb" PROPERTIES FOLDER "external")

# objparser
project(OBJPARSER)
set(OBJPARSER_DIR "${DEPENDENCIES_PATH}/objparser")
add_library("objparser" "${OBJPARSER_DIR}/objparser.cpp")
target_include_directories("objparser" PRIVATE "${OBJPARSER_DIR}")
set_target_properties("objparser" PROPERTIES FOLDER "external")

add_subdirectory(meshoptimizer)
if(APPLE)
    target_compile_options(meshoptimizer PRIVATE -Wno-comma)
endif(APPLE)
set_target_properties(meshoptimizer PROPERTIES FOLDER "external")
