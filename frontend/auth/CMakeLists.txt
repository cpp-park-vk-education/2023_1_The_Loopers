#add_library(${AUTH} STATIC "")

#set_target_properties(${AUTH} PROPERTIES AUTOMOC TRUE)

set(SOURCES
        "src/AuthView.cpp"
        "src/LoginView.cpp"
        "src/AuthModel.cpp"
)

# target_include_libs(${AUTH} PRIVATE
#         "${SERIALIZER}"
#         }

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

file(GLOB HEADER_FILES include/*.h)

qt_wrap_cpp(SOURCES
        ${HEADER_FILES}
        )

qt_add_library(${AUTH}
        STATIC
        "${SOURCES}"
        )

target_include_directories(${AUTH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${AUTH} PRIVATE 
        "${REQUIRED_LIBS_QUALIFIED}"
        "${CLIENT_NETWORK}"
        "${NETWORK_ADAPTER}"
        "${SERIALIZER}"
        "${CLIENT_NETWORK}"
)
