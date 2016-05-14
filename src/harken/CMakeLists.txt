include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED)

add_library(${LIB_NAME} STATIC
    sdl.cpp
)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${LIB_NAME})
