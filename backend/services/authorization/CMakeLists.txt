add_executable(${AUTHORIZATION_SERVICE} "")

target_sources(${AUTHORIZATION_SERVICE}
PRIVATE
    #"src/main.cpp"
    "src/auth_handler.cpp"
	"src/auth_handler_factory.cpp"
	"src/create_handler.cpp"
	"src/encrypter.cpp"
	"src/log_in_handler.cpp"
)

set(PRIVATE_INCLUDE_DIRECTORIES
    "${UTILS_DIR}/include"
    "include"
)

target_include_directories(${AUTHORIZATION_SERVICE} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

set(PRIVATE_LINK_LIBRARIES
    "${DB_ADAPTER}"
    "${EXTERNAL_SERVICE_CHASSIS}"
    "${CHASSIS_CONFIGURATOR}"
)

target_link_libraries(${AUTHORIZATION_SERVICE} PRIVATE ${PRIVATE_LINK_LIBRARIES})