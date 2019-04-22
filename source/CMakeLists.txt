project(lib3d)

set(LIB_NAME lib3d)

add_definitions(-DTW_STATIC)
add_definitions(-DTW_NO_LIB_PRAGMA)
add_definitions(-DTW_NO_DIRECT3D)
add_definitions(-DGLEW_STATIC)

include_directories(${ROOT_PATH}/source)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLFW_SOURCE_DIR}/deps)
include_directories(${DEPENDENCIES_PATH})
include_directories(${DEPENDENCIES_PATH}/glm)

set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source)

file(GLOB RENDERER_CORE_HEADER "*.h")
file(GLOB RENDERER_CORE_SOURCE "*.cpp")
set(RENDERER_CORE ${RENDERER_CORE_HEADER} ${RENDERER_CORE_SOURCE})
source_group("Common" FILES ${RENDERER_CORE})

option(BUILD_OPENGL "Legacy OpenGL" ON)

if(APPLE)
    option(BUILD_METAL "Apple Metal" ON)
endif()

if(BUILD_OPENGL)
	find_package(OpenGL REQUIRED)

    file(GLOB RENDERER_OGL_HEADER "OpenGL/*.h")
    file(GLOB RENDERER_OGL_SOURCE "OpenGL/*.cpp")

    set(RENDERER_OGL ${RENDERER_OGL_HEADER} ${RENDERER_OGL_SOURCE})

    source_group("OpenGL" FILES ${RENDERER_OGL})
endif()

if(BUILD_METAL)
    file(GLOB RENDERER_MTL_HEADER "Metal/*.h" "Metal/*.hpp")
    file(GLOB RENDERER_MTL_SOURCE "Metal/*.cpp" "Metal/*.mm")

    set(RENDERER_MTL ${RENDERER_MTL_HEADER} ${RENDERER_MTL_SOURCE})

	source_group("Metal" FILES ${RENDERER_OGL})
endif()

set(RENDERER_LIST ${RENDERER_CORE})

if (BUILD_OPENGL)
    set(RENDERER_LIST ${RENDERER_LIST} ${RENDERER_OGL})
endif()

if(BUILD_METAL)
    set(RENDERER_LIST ${RENDERER_LIST} ${RENDERER_MTL})
endif()

# GLAD
set(GLAD_DIR "${DEPENDENCIES_PATH}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")

# STB
set(STB_DIR "${DEPENDENCIES_PATH}/stb")
add_library("stb" "${STB_DIR}/stb_image.cpp")
target_include_directories("stb" PRIVATE "${STB_DIR}")

add_library(${LIB_NAME} ${RENDERER_LIST})

target_link_libraries(${LIB_NAME} PRIVATE glfw)
target_link_libraries(${LIB_NAME} PRIVATE glad)
target_link_libraries(${LIB_NAME} PRIVATE stb)

target_include_directories(${LIB_NAME} PRIVATE "${GLAD_DIR}/include") 
target_include_directories(${LIB_NAME} PRIVATE "${STB_DIR}")
