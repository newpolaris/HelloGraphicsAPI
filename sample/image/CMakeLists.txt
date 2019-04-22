project("image")
set(APP_TARGET "image")

add_executable(${APP_TARGET} "main.cpp")
target_link_libraries(${APP_TARGET} PRIVATE "lib3d")
set_target_properties(${APP_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${APP_TARGET} PROPERTIES PROJECT_LABEL ${APP_TARGET})
set_target_properties(${APP_TARGET} PROPERTIES FOLDER "sample")
