cmake_minimum_required(VERSION 3.10)
project(Module2)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Module2
    src/Exigence.cpp
    src/ExigenceSDD.cpp
    src/ExigenceSRS.cpp
    src/ExigenceSSS.cpp
    src/MoteurTracabilite.cpp
    src/ThreadManager.cpp
    src/Main.cpp
)
