add_library(${AUTH} STATIC "")

target_sources(${AUTH} PRIVATE
        "src/AuthView.cpp"
        )

set(CMAKE_PREFIX_PATH "/home/azhok/Qt2/6.5.0/gcc_64")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(PUBLIC_INCLUDE_DIRECTORIES
        "include"
        )

target_include_directories(${AUTH} PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES})

target_link_libraries(${AUTH} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        )
