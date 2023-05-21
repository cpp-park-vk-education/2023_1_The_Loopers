# find third party libraries (qt)
set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets)

find_package(Qt6 REQUIRED COMPONENTS ${REQUIRED_LIBS})

qt_standard_project_setup()

target_link_libraries(${GRAPH} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
target_link_libraries(${MENU} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
target_link_libraries(${DRAW} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
target_link_libraries(${AUTH} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
target_link_libraries(${GENERAL} PRIVATE ${REQUIRED_LIBS_QUALIFIED})
