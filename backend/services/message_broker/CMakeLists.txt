add_executable(${MESSAGE_BROKER_SERVICE} "")

target_sources(${MESSAGE_BROKER_SERVICE}
PRIVATE
    "src/main.cpp"
    "src/message_broker_service.cpp"
    "signal_handler/src/isignals_handler.cpp"
    "events_handler/src/ievents_handler.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "events_handler/include"
    "signal_handler/include"
    "include"
)

target_include_directories(${MESSAGE_BROKER_SERVICE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "${BASE_SERVICE_CHASSIS}"
    "${CHASSIS_CONFIGURATOR}"
    "${SERIALIZER}"
    "${AUTHORIZER}"
)

target_link_libraries(${MESSAGE_BROKER_SERVICE} PRIVATE ${PRIVATE_LINK_LIBRARIES})
