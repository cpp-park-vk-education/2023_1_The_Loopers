set(MESSAGE_BROKER_SERVICE service_${PROJECT_NAME}_message_broker)

add_executable(${MESSAGE_BROKER_SERVICE} "")
# target_sources(${PROJECT_NAME}
# PRIVATE
# "src/main.cpp"
# "src/solve_hw1.cpp"
# )

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_FOLDER}/include"
    "events_handler/include"
    "signal_handler/include"
    "include"
)

target_include_directories(${MESSAGE_BROKER_SERVICE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "$ENV{EXTERNAL_SERVICE_CHASSISS}"
    "$ENV{CHASSISS_CONFIGURATOR}"
)

target_link_libraries(${MESSAGE_BROKER_SERVICE} PRIVATE ${PRIVATE_LINK_LIBRARIES})
