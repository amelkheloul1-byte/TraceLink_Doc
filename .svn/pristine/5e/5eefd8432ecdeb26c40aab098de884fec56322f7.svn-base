cmake_minimum_required(VERSION 3.19)
project(L2G1_Module3 LANGUAGES CXX)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()
qt_add_executable(L2G1_Module3
    WIN32 MACOSX_BUNDLE
    sources/main.cpp
    sources/mainwindow.cpp
    sources/mainwindow.ui
    sources/accueil.cpp
    headers/mainwindow.h
    headers/accueil.h
)
target_link_libraries(L2G1_Module3
    PRIVATE
        Qt::Core
        Qt::Widgets
)
install(TARGETS L2G1_Module3
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
qt_generate_deploy_app_script(
    TARGET L2G1_Module3
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})