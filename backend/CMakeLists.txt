set(BACKEND_UTILS_DIR "${CMAKE_CURRENT_LIST_DIR}/utils")

add_subdirectory(utils/token_generator)
add_subdirectory(utils/authorizer)
add_subdirectory(utils/server_network)

add_subdirectory(utils/db_adapter)

add_subdirectory(utils/base_service_chassis)
add_subdirectory(utils/external_service_chassis)
add_subdirectory(utils/service_chassis_configurators)

add_subdirectory(services/simultaneous_access)
<<<<<<< HEAD
add_subdirectory(services/storage)
=======
add_subdirectory(services/service_registry)
add_subdirectory(services/message_broker)
>>>>>>> 4b2812f08a77063d2216124b452301cd62c94b59
