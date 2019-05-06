project(objparser)

set(OBJPARSER_DIR "${DEPENDENCIES_PATH}/objparser")
add_library("objparser" "${OBJPARSER_DIR}/objparser.cpp")
target_include_directories("objparser" PRIVATE "${OBJPARSER_DIR}")
