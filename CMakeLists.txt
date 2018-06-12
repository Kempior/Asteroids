cmake_minimum_required(VERSION 3.10)
project(Asteroids)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(Asteroids
        src/Asteroids.cpp
        include/Asteroids/Application.hpp src/Application.cpp
        include/Asteroids/States/State.hpp src/States/State.cpp
        include/Asteroids/States/StateGame.hpp src/States/StateGame.cpp
        include/Asteroids/States/StateMainMenu.hpp src/States/StateMainMenu.cpp
        include/Asteroids/GUI/Box.hpp src/GUI/Box.cpp
        include/Asteroids/GUI/Button.hpp src/GUI/Button.cpp
        include/Asteroids/GUI/Canvas.hpp src/GUI/Canvas.cpp
        include/Asteroids/GUI/Label.hpp src/GUI/Label.cpp
        include/Asteroids/GUI/TextField.hpp src/GUI/TextField.cpp
        include/Asteroids/GUI/Widget.hpp src/GUI/Widget.cpp
        include/Asteroids/GUI/Widgets.hpp
        include/Asteroids/ServiceLocator/ResourceFont.hpp
        include/Asteroids/ServiceLocator/DefaultResourceFont.hpp src/ServiceLocator/DefaultResourceFont.cpp
        include/Asteroids/ServiceLocator/Locator.hpp src/ServiceLocator/Locator.cpp
        )

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