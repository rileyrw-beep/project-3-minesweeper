//
// Created by riley on 12/6/2025.
//

#include "Button.hpp"

Button::Button(std::string fn, float x, float y) : fileName(std::move(fn)) {
    std::string fullPath = "files/images/" + fileName + ".png";
    if (!buttonTexture.loadFromFile(fullPath)) { std::exit(1); }
    buttonSprite.setTexture(buttonTexture);
    buttonSprite.setPosition(x, y);
}

void Button::displayButton(sf::RenderWindow& window) {
    window.draw(buttonSprite);
}

bool Button::mousePress(int x, int y) {
    if (buttonSprite.getGlobalBounds().contains(x, y)) {
        return true;
    }
    return false;
}

void Button::updateGraphic(const std::string& newFN) {
    std::string fullPath = "files/images/" + newFN + ".png";
    if (!buttonTexture.loadFromFile(fullPath)) { std::exit(1); }
}