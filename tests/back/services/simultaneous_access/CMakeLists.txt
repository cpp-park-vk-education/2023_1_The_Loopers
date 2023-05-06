set(TEST_EXECUTABLE test_simultaneous_access)

add_executable(${TEST_EXECUTABLE} "")

# add_subdirectory(ProtocolDecoder)
add_subdirectory(resolvers)

# target_sources(${TEST_EXECUTABLE}
# PRIVATE
# "test_SimultaneousAccessService.cpp"
# )
set(PRIVATE_INCLUDE_DIRECTORIES
    "${PROJECT_SOURCE_DIR}/back/services/simultaneous_access/resolvers/draw_conflict_resolver/include"
    "${PROJECT_SOURCE_DIR}/back/services/simultaneous_access/resolvers/text_conflict_resolver/include"
    "${PROJECT_SOURCE_DIR}/utls"
    "${PROJECT_SOURCE_DIR}/serializer_deserializer/include"
)

target_include_directories(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "GTest::gtest_main"

    # "Back"
)

target_link_libraries(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_LINK_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE})
