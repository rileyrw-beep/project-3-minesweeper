//
// Created by riley on 12/8/2025.
//

#ifndef MINESWEEPERPROJECT_LEADERBOARDWINDOW_HPP
#define MINESWEEPERPROJECT_LEADERBOARDWINDOW_HPP
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include "config.hpp"
#include <iostream>
#include "Board.hpp"
#include "Button.hpp"
#include "PossibleOutcomes.hpp"

class LeaderboardWindow {
public:
    LeaderboardWindow(const sf::Font& font, const config& config);
    void Run();
    void setText(sf::Text& text, float x, float y);
    void readFromFile(); //read into records
    void writeToFile(std::string stringToWrite);//write to file




private:
    unsigned int width, height;
    float textWidth, textHeight;
    sf::RenderWindow leaderboardWindow;
    std::string records;
    std::string currentName;
    std::string filename;
    sf::Text leaderboardText;
    sf::Text leaderboardContents;
    sf::Font font;

};


#endif //MINESWEEPERPROJECT_LEADERBOARDWINDOW_HPP