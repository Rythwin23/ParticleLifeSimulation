cmake_minimum_required(VERSION 3.28)
project(ParticleLifeSimulation)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

add_executable(ParticleLifeSimulation main.cpp
        main.h
        Particule.cpp
        Particule.h
)


set(SFML_STATIC_LIBRARIES TIME)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include)

target_link_libraries(ParticleLifeSimulation sfml-system sfml-window sfml-graphics sfml-audio)