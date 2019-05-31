project(lib3d)

set(LIB_NAME lib3d)

include_directories(${GLFW_SOURCE_DIR}/deps)

set(HEADER_PATH ${CMAKE_SOURCE_DIR}/include/el)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source/lib3d)

set(RENDERER_CORE_HEADER
    ${HEADER_PATH}/graphics_context.h
    ${HEADER_PATH}/graphics_data.h
    ${HEADER_PATH}/graphics_depth_stencil.h
    ${HEADER_PATH}/graphics_device.h
    ${HEADER_PATH}/graphics_driver.h
    ${HEADER_PATH}/graphics_framebuffer.h
    ${HEADER_PATH}/graphics_input_layout.h
    ${HEADER_PATH}/graphics_pipeline.h
    ${HEADER_PATH}/graphics_platform.h
    ${HEADER_PATH}/graphics_program.h
    ${HEADER_PATH}/graphics_sampler.h
    ${HEADER_PATH}/graphics_shader.h
    ${HEADER_PATH}/graphics_texture.h
    ${HEADER_PATH}/graphics_types.h)

file(GLOB RENDERER_CORE_SOURCE "*.cpp")
set(RENDERER_CORE ${RENDERER_CORE_HEADER} ${RENDERER_CORE_SOURCE})
source_group("Common" FILES ${RENDERER_CORE})

if(BUILD_OPENGL)
    find_package(OpenGL REQUIRED)

    file(GLOB RENDERER_OGL_HEADER "OpenGL/*.h")
    file(GLOB RENDERER_OGL_SOURCE "OpenGL/*.cpp")

    set(RENDERER_OGL ${RENDERER_OGL_HEADER} ${RENDERER_OGL_SOURCE})

    source_group("OpenGL" FILES ${RENDERER_OGL})
endif()

if(BUILD_METAL)
    file(GLOB RENDERER_MTL_HEADER "Metal/*.h")
    file(GLOB RENDERER_MTL_SOURCE "Metal/*.cpp" "Metal/*.mm")

    set(RENDERER_MTL ${RENDERER_MTL_HEADER} ${RENDERER_MTL_SOURCE})
    source_group("Metal" FILES ${RENDERER_MTL})
endif()

set(RENDERER_LIST ${RENDERER_CORE})

if(BUILD_OPENGL)
    set(RENDERER_LIST ${RENDERER_LIST} ${RENDERER_OGL})
endif()

if(BUILD_METAL)
    set(RENDERER_LIST ${RENDERER_LIST} ${RENDERER_MTL})
endif()

add_library(${LIB_NAME} ${RENDERER_LIST})

if(BUILD_OPENGL)
    target_compile_definitions(${LIB_NAME} PRIVATE -DEL_BUILD_OPENGL=1)
endif()
if(BUILD_METAL)
    target_compile_definitions(${LIB_NAME} PRIVATE -DEL_BUILD_METAL=1)
endif()

target_link_libraries(${LIB_NAME} PRIVATE glad)

target_include_directories(${LIB_NAME} PRIVATE ${glad_SOURCE_DIR}/include)

set_target_properties(${LIB_NAME} PROPERTIES FOLDER "core")
