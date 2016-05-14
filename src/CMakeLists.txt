set(CMAKE_CXX_FLAGS "-std=c++14")

find_package(OpenGL)
find_package(GLEW)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED)

set(APP_NAME app)
set(LIB_NAME harken)

add_subdirectory(${LIB_NAME})

set(APP_SOURCES 
    main.cpp
)

include_directories(. ${SDL2_INCLUDE_DIRS})

add_executable(${APP_NAME} ${APP_SOURCES})
target_link_libraries(${APP_NAME} ${LIB_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SDL2_LIBRARIES})

add_subdirectory(test)