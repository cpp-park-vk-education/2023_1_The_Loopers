# find third party libraries (qt)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


set(QT_COMPONENTS
        "Qt6::Core"
        "Qt6::Gui"
        "Qt6::Widgets"
        )

qt_standard_project_setup()
