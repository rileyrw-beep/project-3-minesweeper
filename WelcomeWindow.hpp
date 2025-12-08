//
// Created by riley on 12/1/2025.
//
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <cstdint>
#include "config.hpp"

#ifndef MINESWEEPERPROJECT_WELCOMEWINDOW_HPP
#define MINESWEEPERPROJECT_WELCOMEWINDOW_HPP



class WelcomeWindow {
public:
        WelcomeWindow(const sf::Font& font, const config& config);
        bool Run(const config& config);
        void setText(sf::Text& text, float x, float y);
        void setDynamicText(sf::Text& text, float x, float y);
        std::string getName() const;

private:
        unsigned int width;
        unsigned int height;
        float textWidth;
        float textHeight;
        sf::Text welcomeText;
        sf::Text enterNameText;
        std::string userString;
        sf::Text userText;
        sf::RectangleShape cursor;
        sf::RenderWindow welcomeWindow;
};


#endif //MINESWEEPERPROJECT_WELCOMEWINDOW_HPP