project("sharing")
set(APP_TARGET "sharing")

FILE(GLOB HEADER_LIST *.h)
FILE(GLOB SOURCE_LIST *.cpp)

source_group("sharing" FILES ${HEADER_LIST})
source_group("sharing" FILES ${SOURCE_LIST})

add_executable(${APP_TARGET} ${HEADER_LIST} ${SOURCE_LIST})

target_link_libraries(${APP_TARGET} PRIVATE "lib3d")
