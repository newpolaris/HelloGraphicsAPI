set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source)

file(GLOB RENDERER_CORE_HEADER "*.h")
file(GLOB RENDERER_CORE_SOURCE "*.cpp")
set(RENDERER_CORE ${RENDERER_CORE_HEADER} ${RENDERER_CORE_SOURCE})
source_group("Common" FILES ${RENDERER_CORE})

option(BUILD_OPENGL "Legacy OpenGL" ON)

if(APPLE)
    option(BUILD_METAL "Apple Metal" ON)
endif()

if(BUILD_OPENGL)
    file(GLOB RENDERER_OGL_HEADER "OpenGL/*.h")
    file(GLOB RENDERER_OGL_SOURCE "OpenGL/*.cpp")

    set(RENDERER_OGL ${RENDERER_OGL_HEADER} ${RENDERER_OGL_SOURCE})

    source_group("OpenGL" FILES ${RENDERER_OGL})
endif()

if(BUILD_METAL)
    file(GLOB RENDERER_MTL_HEADER "Metal/*.h" "Metal/*.hpp")
    file(GLOB RENDERER_MTL_SOURCE "Metal/*.cpp" "Metal/*.mm")

    set(RENDERER_MTL ${RENDERER_MTL_HEADER} ${RENDERER_MTL_SOURCE})

    source_group("OpenGL" FILES ${RENDERER_OGL})
endif()

set(RENDERER_LIST ${RENDERER_CORE})

if (BUILD_OPENGL)
    set(RENDERER_LIST ${RENDERER_LIST} ${RENDERER_OGL})
endif()

if(BUILD_METAL)
    set(RENDERER_LIST ${RENDERER_LIST} ${RENDERER_MTL})
endif()

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

set(OSX_SOURCE
    OSX/AppDelegate.mm
    OSX/GameViewController.mm
    OSX/main.mm
    OSX/AppDelegate.h
    OSX/GameViewController.h
    OSX/SharedStructures.h)

add_executable(
    ${APP_TARGET} 
    #MACOSX_BUNDLE 
    ${RENDERER_LIST} 
    ${GLAD} 
    ${STB}
    #${OSX_SOURCE} 
    #OSX/MainMenu.xib 
    #OSX/Shaders.metal
    )

target_include_directories(${APP_TARGET} PRIVATE "${GLAD_DIR}/include" "{STB_DIR}")
target_link_libraries(${APP_TARGET} ${ALL_LIBS})

# Xcode and Visual working directories
if (APPLE)
	set_target_properties(${APP_TARGET} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
endif()
create_target_launcher(${APP_TARGET} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
