project(libplatform)

set(LIB_NAME libplatform)

set(HEADER_PATH ${CMAKE_SOURCE_DIR}/include/el)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source/libplatform)

set(PLATFORM_CORE_LIST
    ${HEADER_PATH}/predefines.h
    ${HEADER_PATH}/casting.h
    ${HEADER_PATH}/debug.h
    ${SOURCE_PATH}/debug.cpp
    ${HEADER_PATH}/types.h
    ${HEADER_PATH}/image.h
    ${SOURCE_PATH}/image.cpp
    ${HEADER_PATH}/platform.h
    ${HEADER_PATH}/utility.h
    ${SOURCE_PATH}/utility.cpp
    ${HEADER_PATH}/math.h)

set(PLATFORM_CORE ${PLATFORM_CORE_LIST})
source_group("Core" FILES ${PLATFORM_CORE_LIST})

set(PLATFORM_LIST ${PLATFORM_CORE})

add_library(${LIB_NAME} ${PLATFORM_LIST})

target_link_libraries(${LIB_NAME} PRIVATE stb)
target_include_directories(${LIB_NAME} PRIVATE ${STB_SOURCE_DIR})

set_target_properties(${LIB_NAME} PROPERTIES FOLDER "core")
