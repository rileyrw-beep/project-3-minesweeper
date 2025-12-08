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
    //

    leaderboardText.setFont(font);
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setCharacterSize(20);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboardText, textWidth, textHeight - 120.0f);

    leaderboardContents.setFont(font);
    leaderboardContents.setCharacterSize(18);
    leaderboardContents.setStyle(sf::Text::Bold);
    leaderboardContents.setFillColor(sf::Color::White);
    setText(leaderboardContents, textWidth, textHeight + 20.0f);
}


void LeaderboardWindow::Run() {
    leaderboardWindow.create(sf::VideoMode(width, height), "Leaderboard Window");
    //game loop
    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        //get the event
        while (leaderboardWindow.pollEvent(event)) {
            //if click the x in the corner, end without error
            if (event.type == sf::Event::Closed) {
                leaderboardWindow.close();
                return;
            }
        }

        leaderboardContents.setString(records);

        leaderboardWindow.clear(sf::Color::Blue); //set window background blue
        leaderboardWindow.draw(leaderboardText);
        leaderboardWindow.draw(leaderboardContents);


        leaderboardWindow.display(); //display
    }
    return;
}


void LeaderboardWindow::setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}


