add_library(${AUTH} STATIC "")

set_target_properties(${AUTH} PROPERTIES AUTOMOC TRUE)

target_sources(${AUTH} PRIVATE
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

target_include_directories(${AUTH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${AUTH} PRIVATE 
        ${REQUIRED_LIBS_QUALIFIED}
        ${SERIALIZER}
)
