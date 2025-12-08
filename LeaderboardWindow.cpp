//
// Created by riley on 12/8/2025.
//

#include "LeaderboardWindow.hpp"

LeaderboardWindow::LeaderboardWindow(const sf::Font& font, const config &config) {
    width = config.colCount*16;
    height = config.rowCount*16+50;
    textWidth = static_cast<float>(width) / 2.0f;
    textHeight = static_cast<float>(height) / 2.0f;
    this->font = font;
    leaderboardWindow.create(sf::VideoMode(width, height), "Leaderboard Window");

}


