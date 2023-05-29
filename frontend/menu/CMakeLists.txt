# add_library(${MENU} STATIC "")

set(SOURCES
        "src/temp.cpp"
) 

# target_sources(${MENU} PRIVATE
#         "${SOURCES}"
#         )

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

qt_wrap_cpp(SOURCES 
        include/MenuView.h
        include/MenuModel.h
)

qt_add_library(${MENU}
        STATIC
    "${SOURCES}"
)


target_include_directories(${MENU} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${MENU} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
