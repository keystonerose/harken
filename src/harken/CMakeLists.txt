include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

add_library(${LIB_NAME} STATIC
    exception.cpp
    sdl.cpp
    shader.cpp
    shaderprogram.cpp
)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${LIB_NAME})
