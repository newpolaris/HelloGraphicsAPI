project("image")
set(APP_TARGET "image")

add_executable(${APP_TARGET} "main.cpp")
target_link_libraries(${APP_TARGET} PRIVATE "lib3d")
