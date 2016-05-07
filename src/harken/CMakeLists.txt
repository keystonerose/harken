include_directories(${FREETYPE_INCLUDE_DIRS})

add_library(${LIB_NAME} STATIC
    placeholder.cpp
)

target_link_libraries(${LIB_NAME})
