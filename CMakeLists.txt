cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)
if (${CMAKE_VERSION} VERSION_GREATER "3.24")
    cmake_policy(SET CMP0135 OLD)
endif()

project(inklink VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/get_boost_lib.cmake)

option(ENABLE_BACKEND "Set to \"ON\" to build back" ON)
option(ENABLE_FRONTEND "Set to \"OFF\" to build front" OFF)

option(CLEAR_CACHE "Set to \"ON\" to clear cache" ON)

option(ENABLE_TEST "Set to \"OFF\" for test build" OFF)

option(ENABLE_CLANG_FORMAT "Set to \"ON\" to add clang-format build target" OFF)

option(ENABLE_CLANG_TIDY "Set to \"ON\" to enable clang-tidy" OFF)
option(ENABLE_CPPLINT "Set to \"ON\" to enable cpplint" OFF)
option(ENABLE_CPPCHECK "Set to \"ON\" to enable cppcheck" OFF)

option(ENABLE_SANITIZER "Set to \"ON\" to enable sanitizer in debug build" OFF)

set(UTILS_DIR "${PROJECT_SOURCE_DIR}/utils")

include(cmake/apply_options.cmake)

# services
set(MESSAGE_BROKER_SERVICE service_${PROJECT_NAME}_message_broker)
set(SIMULTANEOUS_ACCESS_SERVICE service_${PROJECT_NAME}_simultaneous_access)
<<<<<<< HEAD
set(STORAGE_SERVICE service_${PROJECT_NAME}_storage_service)

=======
set(SERVICE_REGISTRY service_${PROJECT_NAME}_service_registry)
>>>>>>> 4b2812f08a77063d2216124b452301cd62c94b59
# backend utils
set(AUTHORIZER ${PROJECT_NAME}_authorizer)
set(BASE_SERVICE_CHASSIS ${PROJECT_NAME}_base_service_chassis)
set(DB_ADAPTER ${PROJECT_NAME}_db_adapter)
set(EXTERNAL_SERVICE_CHASSIS ${PROJECT_NAME}_external_service_chassis)
set(SERVER_NETWORK ${PROJECT_NAME}_server_network)
set(CHASSIS_CONFIGURATOR ${PROJECT_NAME}_services_chassis_configurator)
set(TOKEN_GENERATOR ${PROJECT_NAME}_token_generator)

# global utils
set(CLIENT_NETWORK ${PROJECT_NAME}_client_network)
set(NETWORK_ADAPTER ${PROJECT_NAME}_network_adapter)
set(SERIALIZER ${PROJECT_NAME}_serializer)

# frontend
set(AUTH ${PROJECT_NAME}_AUTH)
set(DRAW ${PROJECT_NAME}_DRAW)
set(GRAPH ${PROJECT_NAME}_GRAPH)
set(MENU ${PROJECT_NAME}_MENU)
set(GENERAL ${PROJECT_NAME}_GENERAL)

add_subdirectory(${UTILS_DIR})
if (ENABLE_BACKEND)
    add_subdirectory(backend)
endif()
if (ENABLE_FRONTEND)
    include(cmake/apply_qt.cmake)
    add_subdirectory(frontend)
endif()
if (ENABLE_TEST)
    add_subdirectory(tests)
endif()

if (CLEAR_CACHE)
    unset(ENABLE_TEST CACHE)

    unset(ENABLE_CLANG_FORMAT CACHE)
    unset(ENABLE_CPPLINT CACHE)
    unset(ENABLE_CLANG_TIDY CACHE)
    unset(ENABLE_CPPCHECK CACHE)
    
    unset(ENABLE_SANITIZER CACHE)
endif()
