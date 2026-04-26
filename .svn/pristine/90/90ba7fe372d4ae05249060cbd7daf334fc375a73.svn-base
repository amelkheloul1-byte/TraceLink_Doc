cmake_minimum_required(VERSION 3.19)
project(L2G1_Module3 LANGUAGES CXX)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()
qt_add_executable(L2G1_Module3
    WIN32 MACOSX_BUNDLE
    # --- MODULE 3 ---
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/accueil.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/OngletRapport.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/VueGrapheTracabilite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/GestionnaireFiltresGUI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/accueil.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OngletRapport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/VueGrapheTracabilite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/GestionnaireFiltresGUI.cpp
    # --- INTEGRATION M1M2 ---
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/ExtracteurFichier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/ConvertisseurExigence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/Exigence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/ExigenceSSS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/ExigenceSRS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/ExigenceSDD.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/MoteurTracabilite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/RapportTracabilite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include/ThreadManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/ExtracteurFichier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/ConvertisseurExigence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/Exigence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/ExigenceSSS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/ExigenceSRS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/ExigenceSDD.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/MoteurTracabilite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/src/ThreadManager.cpp
)
target_compile_definitions(L2G1_Module3
    PRIVATE
        _FILE_OFFSET_BITS=64
)
target_include_directories(L2G1_Module3
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
        ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include
        # AJOUT : headers minizip et tinyxml2 mingw64
        C:/msys64/mingw64/include
)
target_link_libraries(L2G1_Module3
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        # AJOUT : bibliothèques minizip mingw64 et dépendances
        C:/msys64/mingw64/lib/libzip.dll.a
        C:/msys64/mingw64/lib/libpugixml.a
        C:/msys64/mingw64/lib/libz.a
        C:/msys64/mingw64/lib/libbz2.a
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
