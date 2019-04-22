project("sharing")
set(APP_TARGET "sharing")

FILE(GLOB HEADER_LIST *.h)
FILE(GLOB SOURCE_LIST *.cpp)

source_group("sharing" FILES ${HEADER_LIST})
source_group("sharing" FILES ${SOURCE_LIST})

add_executable(${APP_TARGET} ${HEADER_LIST} ${SOURCE_LIST})

target_link_libraries(${APP_TARGET} PRIVATE "lib3d")
set_target_properties(${APP_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${APP_TARGET} PROPERTIES PROJECT_LABEL ${APP_TARGET})
set_target_properties(${APP_TARGET} PROPERTIES FOLDER "sample")
