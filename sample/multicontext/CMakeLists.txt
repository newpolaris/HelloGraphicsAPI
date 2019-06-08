project(multicontext)

set(SAMPLE_NAME ${PROJECT_NAME})
set(SAMPLE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Generating project file for example in ${SAMPLE_FOLDER}")

set(SOURCE_LIST 
    ${SAMPLE_FOLDER}/platform.h
    ${SAMPLE_FOLDER}/main.cpp)

if(WIN32)
    set(SOURCE_LIST 
        ${SOURCE_LIST}
        ${SAMPLE_FOLDER}/platform_wgl.h
        ${SAMPLE_FOLDER}/platform_wgl.cpp)
endif()
if(APPLE)
    set(SOURCE_LIST 
        ${SOURCE_LIST}
        ${SAMPLE_FOLDER}/platform_nsgl.h
        ${SAMPLE_FOLDER}/platform_nsgl.mm)
endif()

source_group("sources" FILES ${SOURCE_LIST})
add_executable(${SAMPLE_NAME} ${SOURCE_LIST})

target_link_libraries(${SAMPLE_NAME} PRIVATE "lib3d")
target_link_libraries(${SAMPLE_NAME} PRIVATE "libplatform")
target_link_libraries(${SAMPLE_NAME} PRIVATE "objparser")
target_link_libraries(${SAMPLE_NAME} PRIVATE "meshoptimizer")
target_link_libraries(${SAMPLE_NAME} PRIVATE "glfw")
if(APPLE)
    target_link_libraries(${SAMPLE_NAME} PRIVATE "-framework AppKit")
endif()
if(APPLE AND BUILD_METAL)
    target_link_libraries(${SAMPLE_NAME} PRIVATE "-framework Metal")
    target_link_libraries(${SAMPLE_NAME} PRIVATE "-framework QuartzCore")
endif()

set_target_properties(${SAMPLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${SAMPLE_FOLDER})
set_target_properties(${SAMPLE_NAME} PROPERTIES PROJECT_LABEL ${SAMPLE_NAME})
set_target_properties(${SAMPLE_NAME} PROPERTIES FOLDER "sample")
