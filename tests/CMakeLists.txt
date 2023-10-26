set(
    SOURCES
    main.cpp
    profiler.cpp
    test_runner.cpp
    test_utils.cpp
)

add_executable(ortests ${SOURCES})
target_link_libraries(ortests orders)