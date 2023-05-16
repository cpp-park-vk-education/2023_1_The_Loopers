set(TEST_EXECUTABLE test_simultaneous_access)

add_executable(${TEST_EXECUTABLE} "")

set(RESOLVERS_DIR "${SERVICES_DIR}/simultaneous_access/resolvers")

set(PRIVATE_INCLUDE_DIRECTORIES
    "${RESOLVERS_DIR}/draw_conflict_resolver/include"
    "${RESOLVERS_DIR}/text_conflict_resolver/include"
    "${UTILS_DIR}/include"
    "${UTILS_DIR}/serializer_deserializer/include"
)

target_include_directories(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "GTest::gtest_main"
    "GTest::gmock_main"

    # "Back"
)

target_link_libraries(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_LINK_LIBRARIES})

target_sources(${TEST_EXECUTABLE}
    PRIVATE
    "resolvers/draw_conflict_resolver/test_blocking_draw_conflict_resolver.cpp"
    "resolvers/text_conflict_resolver/test_text_conflict_resovler.cpp"

    "${RESOLVERS_DIR}/draw_conflict_resolver/src/blocking_draw_resolver.cpp"
)

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE})
