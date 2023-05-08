set(TEST_EXECUTABLE test_base_service_chassiss)

add_executable(${TEST_EXECUTABLE} "")

target_sources(${TEST_EXECUTABLE} PRIVATE
    "internal_sessions_manager/test_internal_sessions_manager.cpp"
    # "test_base_service_chassiss.cpp" nothing interesting
)

set(PRIVATE_LINK_LIBRARIES
    "GTest::gtest_main"
    "GTest::gmock_main"
    "$ENV{BASE_SERVICE_CHASSISS}"
)

target_link_libraries(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_LINK_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE})
