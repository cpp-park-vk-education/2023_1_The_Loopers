set(TEST_EXECUTABLE test_message_broker)

add_executable(${TEST_EXECUTABLE} "")

target_sources(${TEST_EXECUTABLE}
PRIVATE
    # "resolvers/draw_conflict_resolver/test_draw_conflict_resolver.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
)

target_include_directories(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "GTest::gtest_main"

    # "Back"
)

target_link_libraries(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_LINK_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE})
