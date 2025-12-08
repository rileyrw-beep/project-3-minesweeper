//
// Created by riley on 12/1/2025.
//


#ifndef MINESWEEPERPROJECT_GAMEWINDOW_HPP
#define MINESWEEPERPROJECT_GAMEWINDOW_HPP
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include "config.hpp"
#include "Board.hpp"
#include "Button.hpp"
#include "PossibleOutcomes.hpp"
#include "LeaderboardWindow.hpp"

class GameWindow {

public:
    GameWindow(const sf::Font& font, const config& config, const std::string& name);
    bool Run();
    void displayButtons(sf::RenderWindow& window);
    void win();
    void lose();
    void leftClick(int posX, int posY);
    void rightClick(int posX, int posY);
    void toggle(Button& button, bool& b, int x, int y, const std::string& fName, void(GameWindow::*action)());
    void reset();
    void displayCounter(sf::RenderWindow &window);
    void displayClock(sf::RenderWindow &window);
    void runLeaderboard();


    template <std::size_t N>
    void displayCustomNumber(std::array<sf::Sprite, N>& arrTextures, std::string stringNum, sf::RenderWindow &window, int realNum);




private:
    unsigned int width, height;
    Board board;
    sf::RenderWindow gameWindow;
    bool debugOn;
    bool isPaused;
    bool inLeaderboard;
    Button debugButton;
    Button leaderboardButton;
    Button smileyButton;
    Button pauseButton;
    LeaderboardWindow leaderboard;
    std::string name;
    sf::Texture numbersTexture;
    std::array<sf::Sprite, 4> numbersSprite;
    int flagsPlaced;
    std::unordered_map<int, std::function<void(sf::Sprite&)>> digitManager;
    sf::Clock clock;
    int clockOffset;
    bool gamePausedLastInterval;
    std::array<sf::Sprite, 2> minutesSprite;
    std::array<sf::Sprite, 2> secondsSprite;
    std::pair<int, int> currentTime;


    //so I wanted to get a little fancy and my current idea is the thing that manages whether the game has ended or not
    //will be an unordered map where there keys are enums of either win, lose, or ongoing, and the values
    //are function pointers that point to either win game, end game, or nothing
    //heres where I got the syntax: https://en.cppreference.com/w/cpp/language/enum.html - enum
    //https://en.cppreference.com/w/cpp/language/pointer.html - pointer to member functions section
    PossibleOutcome currentState;
    std::unordered_map<PossibleOutcome, void (GameWindow::*)()> outcomeManager;


};


#endif //MINESWEEPERPROJECT_GAMEWINDOW_HPP