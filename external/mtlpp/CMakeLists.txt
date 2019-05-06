project(mtlpp)

set(MTLPP_DIR "${DEPENDENCIES_PATH}/mtlpp")
add_library("mtlpp" "${MTLPP_DIR}/mtlpp.mm")
target_include_directories("mtlpp" PRIVATE ${MTLPP_DIR})
