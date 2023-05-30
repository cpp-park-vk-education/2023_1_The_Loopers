#add_executable(${GENERAL} "")

#set_target_properties(${GENERAL} PROPERTIES AUTOMOC TRUE)

set(SOURCES
        "src/main.cpp"
        )

set(PRIVATE_INCLUDE_DIRECTORIES
        "include"
        )

file(GLOB HEADER_FILES include/*.h)

qt_wrap_cpp(SOURCES
        ${HEADER_FILES}
        )

qt_add_executable(${GENERAL}
        "${SOURCES}"
        )

target_include_directories(${GENERAL} PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})

target_link_libraries(${GENERAL} PRIVATE 
        ${REQUIRED_LIBS_QUALIFIED} 
        ${AUTH} 
        ${DRAW} 
        ${GRAPH} 
        ${MENU}
)
