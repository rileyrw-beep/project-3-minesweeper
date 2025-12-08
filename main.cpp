#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <cstdint>
#include "WelcomeWindow.hpp"
#include "config.hpp"
#include "GameWindow.hpp"







    auto main()->int {

    //set up the config struct
    config config{};
    std::ifstream in("files/config.cfg");
    if (!in.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 1;
    }
    in >> config.colCount;
    in >> config.rowCount;
    in >> config.bombCount;
    in.close();

    //declare the font for the welcome window
    sf::Font font;
    font.loadFromFile("files/font.ttf");

    WelcomeWindow ww(font, config);
    if (ww.Run(config)) {
        return EXIT_SUCCESS;
    }

    GameWindow gw(font, config, ww.getName());
    if (gw.Run()) {
        return EXIT_SUCCESS;
    }

    return 0;
}

