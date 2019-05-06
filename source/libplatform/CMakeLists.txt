project(libplatform)

set(LIB_NAME libplatform)

set(HEADER_PATH ${CMAKE_SOURCE_DIR}/source/libplatform)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source/libplatform)

set(PLATFORM_CORE_LIST
    ${HEADER_PATH}/el_casting.h
    ${HEADER_PATH}/el_debug.h
    ${SOURCE_PATH}/el_debug.cpp
    ${HEADER_PATH}/el_types.h
    ${HEADER_PATH}/el_image.h
    ${SOURCE_PATH}/el_image.cpp
    ${HEADER_PATH}/el_platform.h
    ${HEADER_PATH}/el_predefine.h
    ${HEADER_PATH}/el_utility.h
    ${SOURCE_PATH}/el_utility.cpp)

set(PLATFORM_CORE ${PLATFORM_CORE_LIST})
source_group("Core" FILES ${PLATFORM_CORE_LIST})

set(PLATFORM_LIST ${PLATFORM_CORE})

add_library(${LIB_NAME} ${PLATFORM_LIST})

target_link_libraries(${LIB_NAME} PRIVATE stb)
target_include_directories(${LIB_NAME} PRIVATE ${STB_SOURCE_DIR})

set_target_properties(${LIB_NAME} PROPERTIES FOLDER "core")
