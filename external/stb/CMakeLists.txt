project(stb)

set(STB_DIR "${DEPENDENCIES_PATH}/stb")
add_library("stb" "${STB_DIR}/stb_image.cpp")
target_include_directories("stb" PRIVATE "${STB_DIR}")
