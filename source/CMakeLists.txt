include_directories(${lib3d_SOURCE_DIR})
include_directories(${libplatform_SOURCE_DIR})
include_directories(${DEPENDENCIES_PATH})
include_directories(${DEPENDENCIES_PATH}/glm)

add_subdirectory(lib3d)
add_subdirectory(libplatform)
