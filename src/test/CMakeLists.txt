find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(TEST_NAME test-all)
set(TEST_SOURCES
    main.cpp
    test_math.cpp
    test_matrix.cpp
    test_vector.cpp
)

add_definitions(-DBOOST_TEST_DYN_LINK)
add_executable(${TEST_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_NAME} ${LIB_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

