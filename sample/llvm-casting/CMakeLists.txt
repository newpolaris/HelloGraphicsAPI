project(llvm-casting)

set(HEADER_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_LIST ${HEADER_PATH}/main.cpp)

source_group("sources" FILES ${SOURCE_LIST})

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE "lib3d")
target_link_libraries(${PROJECT_NAME} PRIVATE "libplatform")

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "sample")
