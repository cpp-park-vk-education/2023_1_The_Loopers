set(BACKEND_UTILS_DIR "${CMAKE_CURRENT_LIST_DIR}/utils")

add_subdirectory(utils/token_generator)
add_subdirectory(utils/authorizer)
add_subdirectory(utils/server_network)

add_subdirectory(utils/db_adapter)

add_subdirectory(utils/base_service_chassis)
add_subdirectory(utils/external_service_chassis)
add_subdirectory(utils/service_chassis_configurators)

add_subdirectory(services/simultaneous_access)
add_subdirectory(services/service_registry)
