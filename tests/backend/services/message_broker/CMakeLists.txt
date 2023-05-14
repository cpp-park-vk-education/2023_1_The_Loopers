set(TEST_EXECUTABLE test_message_broker)

add_executable(${TEST_EXECUTABLE} "")

set(MESSAGE_BROKER_DIR "${PROJECT_SOURCE_DIR}/back/services/message_broker")

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "${MESSAGE_BROKER_DIR}/events_handler/include"
    "${MESSAGE_BROKER_DIR}/signal_handler/include"
)

target_include_directories(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "GTest::gtest_main"
    "GTest::gmock_main"
    "$ENV{BASE_SERVICE_CHASSISS}"
)

target_link_libraries(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_LINK_LIBRARIES})

target_sources(${TEST_EXECUTABLE}
    PRIVATE
    "events_handler/test_events_handler.cpp"
    "signal_handler/test_signal_handler.cpp"

    # "${MESSAGE_BROKER_DIR}/events_handler/src/..."
)

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE})
