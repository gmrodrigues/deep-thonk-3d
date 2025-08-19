# Find the Qt6Test package
find_package(Qt6 COMPONENTS Test REQUIRED)

# Add the test executable
add_executable(deepThonk3d_tests
    main.cpp
    test_engine.cpp
)

# Link the test executable against Qt6::Test and your application's library
target_link_libraries(deepThonk3d_tests
    PRIVATE
        Qt6::Test
        deepThonk3d_lib
)


