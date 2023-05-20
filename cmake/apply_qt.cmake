# find third party libraries (qt)
set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS ${REQUIRED_LIBS})

set(QT_COMPONENTS ${REQUIRED_LIBS_QUALIFIED})

qt_standard_project_setup()
