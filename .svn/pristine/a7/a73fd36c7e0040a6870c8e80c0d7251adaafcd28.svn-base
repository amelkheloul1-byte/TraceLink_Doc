cmake_minimum_required(VERSION 3.10)
project(IntegrationM1M2)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(/opt/homebrew/include)

link_directories(/opt/homebrew/lib)

add_executable(IntegrationM1M2
    src/Exigence.cpp
    src/ExigenceSDD.cpp
    src/ExigenceSRS.cpp
    src/ExigenceSSS.cpp
    src/MoteurTracabilite.cpp
    src/ThreadManager.cpp
    src/ExtracteurFichier.cpp
    src/ConvertisseurExigence.cpp
    src/Main.cpp
)

target_link_libraries(IntegrationM1M2
    minizip
    tinyxml2
)
