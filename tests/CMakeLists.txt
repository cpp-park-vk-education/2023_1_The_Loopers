set(TESTS_BINARY_DIR ${CMAKE_BINARY_DIR}/tests)
file(MAKE_DIRECTORY ${TESTS_BINARY_DIR})

# Create bash script to run test executables
file(WRITE ${TESTS_BINARY_DIR}/run_tests.sh "#!/bin/bash\n")

# example of TEST_EXECUTABLE_PATH: "${CMAKE_BINARY_DIR}/tests/back/services/test_service"
function(add_test_and_update_script TEST_NAME TEST_EXECUTABLE_PATH)
    file(APPEND ${TESTS_BINARY_DIR}/run_tests.sh "echo '' '---------------------------------------'")
    file(APPEND ${TESTS_BINARY_DIR}/run_tests.sh "echo 'Executing test: ''${TEST_NAME}'' ''")
    file(APPEND ${TESTS_BINARY_DIR}/run_tests.sh "echo '---------------------------------------' ''")
    file(APPEND ${TESTS_BINARY_DIR}/run_tests.sh "${TEST_EXECUTABLE_PATH}\n")
endfunction()

if (ENABLE_BACK)
    add_subdirectory(back)
endif()

if (ENABLE_FRONT)
# add_subdirectory(front)
endif()

# Set execute permissions for run_tests.sh
file(
    INSTALL ${TESTS_BINARY_DIR}/run_tests.sh
    DESTINATION ${TESTS_BINARY_DIR}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
)


