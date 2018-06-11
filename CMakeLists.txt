cmake_minimum_required(VERSION 3.10)
project(Asteroids)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(Asteroids src/Asteroids.cpp include/Asteroids/Application.hpp src/Application.cpp include/Asteroids/States/State.hpp src/States/State.cpp include/Asteroids/States/StateGame.hpp src/States/StateGame.cpp)

link_directories(lib)

#Doesn`t work for some reason
#find_library(SFML_NETWORK libsfml-network-d.a)
#find_library(SFML_GRAPHICS libsfml-graphics-d.a)
#find_library(SFML_WINDOW libsfml-window-d.a)
#find_library(SFML_SYSTEM libsfml-system-d.a)

target_link_libraries(Asteroids ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-network-d.a)
target_link_libraries(Asteroids ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-graphics-d.a)
target_link_libraries(Asteroids ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-window-d.a)
target_link_libraries(Asteroids ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsfml-system-d.a)