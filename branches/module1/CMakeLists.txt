cmake_minimum_required(VERSION 3.10)

project(Traçabilité Module extraire)

# Spécifier le standard C++ (C++11 minimum pour std::regex)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Recherche des packages installés
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)
find_package(pugixml REQUIRED)

# 2. Définition de l'exécutable
# tous les fichiers .cpp 
add_executable(extracteur 
    ExtracteurFichier.cpp 
    main.cpp
)

# 3. Inclusion des répertoires de headers
target_include_directories(extracteur PRIVATE 
    ${LIBZIP_INCLUDE_DIRS}
)
target_include_directories(MonProjet PRIVATE 
    Module1 
    /opt/homebrew/include
)

target_link_libraries(MonProjet PRIVATE 
    /opt/homebrew/lib/libzip.dylib
)

# 4. Lien avec les bibliothèques
target_link_libraries(extracteur PRIVATE 
    ${LIBZIP_LIBRARIES}
    pugixml
)

# Message de confirmation pour le debug
message(STATUS "Libzip include dirs: ${LIBZIP_INCLUDE_DIRS}")

# On cherche libzip sur le système
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

# On ajoute le dossier où se trouve zip.h (le "include")
include_directories(${LIBZIP_INCLUDE_DIRS})

# On lie la bibliothèque à ton programme
target_link_libraries(IntegrationM1M2 PRIVATE ${LIBZIP_LIBRARIES})