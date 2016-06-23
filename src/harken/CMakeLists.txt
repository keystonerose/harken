include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

add_library(${LIB_NAME} STATIC
    harken_exception.cpp
    harken_glmath.cpp
    harken_sdl.cpp
    harken_shader.cpp
    harken_shaderprogram.cpp
    harken_vertexarrayobject.cpp
    harken_vertexbufferobject.cpp
)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${LIB_NAME})
