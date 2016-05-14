set(TEST_NAME test-all)
set(TEST_SOURCES
    main.cpp
    tester.cpp
    
)

add_executable(${TEST_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_NAME} ${LIB_NAME})

