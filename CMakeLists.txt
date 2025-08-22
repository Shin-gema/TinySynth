cmake_minimum_required(VERSION 3.10)

project(TinySynth VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Configuration de TinySynth")
message(STATUS "Standard C++: ${CMAKE_CXX_STANDARD}")

include_directories(include)

file(GLOB_RECURSE APP_SOURCES "source/*.cpp")

set(ALL_SOURCES main.cpp ${APP_SOURCES})

find_package(ALSA REQUIRED)

if(ALSA_FOUND)
    message(STATUS "Bibliothèque ALSA trouvée !")
    include_directories(${ALSA_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Bibliothèque ALSA non trouvée. Veuillez l'installer (ex: sudo apt-get install libasound2-dev)")
endif()

add_executable(tinysynth ${ALL_SOURCES})

target_link_libraries(tinysynth PRIVATE ${ALSA_LIBRARIES})

message(STATUS "Configuration terminée. L'exécutable 'tinysynth' sera créé.")
