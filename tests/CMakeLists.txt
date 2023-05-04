set(TEST_EXECUTABLE test)

enable_testing()

add_executable(${TEST_EXECUTABLE} "")
target_sources(${TEST_EXECUTABLE}
    PRIVATE
    "Back/Services/SimultaneousAccess/Resolvers/DrawConflictResolver/test_DrawConflictResolver.cpp"

    # "Back/Services/SimultaneousAccess/Resolvers/TextConflictResolver/test_TextConflictResovler.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${PROJECT_SOURCE_DIR}/Back/Services/SimultaneousAccess/Resolvers/DrawConflictResolver/include"
    "${PROJECT_SOURCE_DIR}/Back/Services/SimultaneousAccess/Resolvers/TextConflictResolver/include"
    "${PROJECT_SOURCE_DIR}/Utilities"
    "${PROJECT_SOURCE_DIR}/SerializerDeserializer/include"
)

target_include_directories(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "GTest::gtest_main"

    # "Back"
)

target_link_libraries(${TEST_EXECUTABLE} PRIVATE ${PRIVATE_LINK_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE})
