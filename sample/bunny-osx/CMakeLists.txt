project(bunny-osx)

set(SAMPLE_NAME ${PROJECT_NAME})
set(SAMPLE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Generating project file for example in ${SAMPLE_FOLDER}")

project(${SAMPLE_NAME})
file(GLOB HEADER_LIST ${SAMPLE_FOLDER}/*.h ${SAMPLE_FOLDER}/*.hpp)
file(GLOB SOURCE_LIST ${SAMPLE_FOLDER}/*.cpp ${SAMPLE_FOLDER}/*.mm)
source_group(${SAMPLE_NAME} FILES ${HEADER_LIST})
source_group(${SAMPLE_NAME} FILES ${SOURCE_LIST})

set(SHADER_DIR ${SAMPLE_FOLDER})
file(GLOB SHADER_HEADERS "${SHADER_DIR}/*.h")
file(GLOB SHADERS "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.frag" "${SHADER_DIR}/*.comp" "${SHADER_DIR}/*.geom" "${SHADER_DIR}/*.tesc" "${SHADER_DIR}/*.tese" "${SHADER_DIR}/*.mesh")
source_group("shaders" FILES ${SHADERS} ${SHADER_HEADERS})

add_executable(${SAMPLE_NAME} ${HEADER_LIST} ${SOURCE_LIST} ${SHADERS} ${SHADER_HEADERS})

target_compile_definitions(${SAMPLE_NAME} PRIVATE -DEL_DEFINE_SAMPLE_PATH=\"${SAMPLE_FOLDER}/\")
target_compile_definitions(${SAMPLE_NAME} PRIVATE -DEL_DEFINE_RESOURCE_PATH=\"${CMAKE_SOURCE_DIR}/resource/\")

target_link_libraries(${SAMPLE_NAME} PRIVATE "lib3d")
target_link_libraries(${SAMPLE_NAME} PRIVATE "libplatform")
target_link_libraries(${SAMPLE_NAME} PRIVATE "objparser")
target_link_libraries(${SAMPLE_NAME} PRIVATE "meshoptimizer")
target_link_libraries(${SAMPLE_NAME} PRIVATE "SDL2")
target_link_libraries(${SAMPLE_NAME} PRIVATE "mtlpp")
if(APPLE)
    target_link_libraries(${SAMPLE_NAME} PRIVATE "-framework AppKit")
endif()
if(APPLE AND BUILD_METAL)
    target_link_libraries(${SAMPLE_NAME} PRIVATE "-framework Metal")
    target_link_libraries(${SAMPLE_NAME} PRIVATE "-framework QuartzCore")
endif()

target_include_directories(${SAMPLE_NAME} PRIVATE "${SDL2_SOURCE_DIR}")
target_include_directories(${SAMPLE_NAME} PRIVATE "${mtlpp_SOURCE_DIR}")

set_target_properties(${SAMPLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${SAMPLE_FOLDER})
set_target_properties(${SAMPLE_NAME} PROPERTIES PROJECT_LABEL ${SAMPLE_NAME})
set_target_properties(${SAMPLE_NAME} PROPERTIES FOLDER "sample")
