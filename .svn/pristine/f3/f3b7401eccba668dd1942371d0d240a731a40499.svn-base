cmake_minimum_required(VERSION 3.19)
project(L2G1_Module3 LANGUAGES CXX)

# On demande Qt6 avec ses modules :
# - Core    : base de Qt (types, threads, etc.)
# - Widgets : interface graphique (fenetres, boutons, tableaux...)
# - PrintSupport : AJOUT camarade — necessaire pour QPrinter et l'export PDF
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets PrintSupport)

qt_standard_project_setup()

qt_add_executable(L2G1_Module3
    WIN32 MACOSX_BUNDLE

    # --- MODULE 3 : fichiers de l'interface graphique ---
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

    # --- INTEGRATION M1M2 : fichiers des modules 1 et 2 ---
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

# Definitions de compilation
target_compile_definitions(L2G1_Module3
    PRIVATE
        _FILE_OFFSET_BITS=64  # Necessaire pour gerer les fichiers de grande taille sur Windows
)

# Dossiers où le compilateur cherche les fichiers .h
target_include_directories(L2G1_Module3
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers                    # Headers du Module 3
        ${CMAKE_CURRENT_SOURCE_DIR}/../IntegrationM1M2/include # Headers des modules 1 et 2
        C:/msys64/mingw64/include                              # Headers minizip et tinyxml2 (mingw64)
)

# Bibliotheques linkees au projet
target_link_libraries(L2G1_Module3
    PRIVATE
        Qt6::Core          # Module Qt de base
        Qt6::Widgets       # Module Qt pour l'interface graphique
        Qt6::PrintSupport  # AJOUT camarade — module Qt pour QPrinter et l'export PDF
        # Bibliotheques tierces installees via mingw64
        C:/msys64/mingw64/lib/libzip.dll.a  # Lecture/ecriture de fichiers ZIP
        C:/msys64/mingw64/lib/libpugixml.a  # Parsing de fichiers XML
        C:/msys64/mingw64/lib/libz.a        # Compression zlib (dependance de libzip)
        C:/msys64/mingw64/lib/libbz2.a      # Compression bzip2 (dependance de libzip)
)

# Installation de l'executable
install(TARGETS L2G1_Module3
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Generation du script de deploiement Qt (copie les DLLs Qt necessaires a cote de l'exe)
qt_generate_deploy_app_script(
    TARGET L2G1_Module3
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
