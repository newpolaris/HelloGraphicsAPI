if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()

include_directories(${glfw_INCLUDE_DIRS} "${GLFW_SOURCE_DIR}/deps")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER "external")

# GLAD
set(GLAD_DIR "${DEPENDENCIES_PATH}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
set_target_properties("glad" PROPERTIES FOLDER "external")

# STB
set(STB_DIR "${DEPENDENCIES_PATH}/stb")
add_library("stb" "${STB_DIR}/stb_image.cpp")
target_include_directories("stb" PRIVATE "${STB_DIR}")
set_target_properties("stb" PROPERTIES FOLDER "external")
