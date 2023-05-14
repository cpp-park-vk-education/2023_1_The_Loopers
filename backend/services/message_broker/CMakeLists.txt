add_executable(${MESSAGE_BROKER_SERVICE} "")

target_sources(${MESSAGE_BROKER_SERVICE}
PRIVATE
    "src/main.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "events_handler/include"
    "signal_handler/include"
    "include"
)

target_include_directories(${MESSAGE_BROKER_SERVICE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "${EXTERNAL_SERVICE_CHASSISS}"
    "${CHASSISS_CONFIGURATOR}"
)

target_link_libraries(${MESSAGE_BROKER_SERVICE} PRIVATE ${PRIVATE_LINK_LIBRARIES})
