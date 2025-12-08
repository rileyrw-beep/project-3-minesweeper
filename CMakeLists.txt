cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(minesweeperProject)


find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
add_executable(minesweeperProject main.cpp
        WelcomeWindow.cpp
        WelcomeWindow.hpp
        config.hpp
        GameWindow.cpp
        GameWindow.hpp
        Tile.cpp
        Tile.hpp
        Board.cpp
        Board.hpp
        Button.cpp
        Button.hpp
        PossibleOutcomes.hpp
        LeaderboardWindow.cpp
        LeaderboardWindow.hpp)
target_link_libraries(minesweeperProject sfml-system sfml-window sfml-graphics sfml-audio)