set(BASE_SERVICE_CHASSISS ${PROJECT_NAME}_base_service_chassiss)

add_library(${BASE_SERVICE_CHASSISS} SHARED "")

target_sources(${BASE_SERVICE_CHASSISS}
PRIVATE
# "src/main.cpp"
# "src/solve_hw1.cpp"
)

# set(PRIVATE_INCLUDE_DIRECTORIES
# "${UTILS_FOLDER}/include"
# )

set(PUBLIC_INCLUDE_DIRECTORIES
    "${UTILS_FOLDER}/include"
    "include"
    "internal_sessions_manager/include"
    "local_service_registry/include"
    "logger_adapter/include"
    "message_broker_connector/include"
    "service_registator/include"
)

target_include_directories(${BASE_SERVICE_CHASSISS} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

# download nlohmann_json library for serializer 
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

# ---------------------------------------------------------------------------------------
# Example of using pre-compiled library
# ---------------------------------------------------------------------------------------
add_executable(example example.cpp)
target_link_libraries(example PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# ---------------------------------------------------------------------------------------
# Example of using header-only library
# ---------------------------------------------------------------------------------------
if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()

set(PRIVATE_LINK_LIBRARIES
    "nlohmann_json::nlohmann_json"
)

target_link_libraries(${BASE_SERVICE_CHASSISS} PRIVATE ${PRIVATE_LINK_LIBRARIES})
