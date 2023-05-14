set(TEST_EXECUTABLE test_simultaneous_access)

add_executable(${TEST_EXECUTABLE} "")

target_sources(${TEST_EXECUTABLE}
    PRIVATE
    "resolvers/draw_conflict_resolver/test_draw_conflict_resolver.cpp"
    "resolvers/text_conflict_resolver/test_text_conflict_resovler.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${SERVICES_DIR}/simultaneous_access/resolvers/draw_conflict_resolver/include"
    "${SERVICES_DIR}/simultaneous_access/resolvers/text_conflict_resolver/include"
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

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE})
