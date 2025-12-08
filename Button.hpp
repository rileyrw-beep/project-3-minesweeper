//
// Created by riley on 12/6/2025.
//

#ifndef MINESWEEPERPROJECT_BUTTON_HPP
#define MINESWEEPERPROJECT_BUTTON_HPP
#include <memory>
#include <vector>
#include <utility>
#include <string>
#include <SFML/Graphics.hpp>
#include "config.hpp"

class Button {
public:
    explicit Button(std::string fn, float x, float y);
    void displayButton(sf::RenderWindow& window);
    void updateGraphic(const std::string& newFN);
    bool mousePress(int x, int y);




protected:
    sf::Texture buttonTexture;
    sf::Sprite buttonSprite;
    std::string fileName;
    //std::pair<float, float> positionPair;
};


#endif //MINESWEEPERPROJECT_BUTTON_HPP