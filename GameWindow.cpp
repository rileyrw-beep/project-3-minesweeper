//
// Created by riley on 12/1/2025.
//

#include "GameWindow.hpp"

#include <iostream>

GameWindow::GameWindow(const sf::Font& font, const config &config, const std::string& name) :
board(config),
debugButton("debug", static_cast<float>(config.colCount) * 32 - 304,32 * (static_cast<float>(config.rowCount) + 0.5f)),
leaderboardButton("leaderboard",static_cast<float>(config.colCount) * 32 - 176,32 * (static_cast<float>(config.rowCount) + 0.5f)),
smileyButton("face_happy", ((static_cast<float>(config.colCount) / 2.0f)*32.0f)-32, 32 * (static_cast<float>(config.rowCount) + 0.5f)),
pauseButton("pause", static_cast<float>(config.colCount) * 32 - 240,32 * (static_cast<float>(config.rowCount) + 0.5f)),
leaderboard(font, config),
name(name)
{
    flagsPlaced = 0;
    if (!numbersTexture.loadFromFile("files/images/digits.png")) {std::exit(1);}
    width = config.colCount * 32;
    height = config.rowCount * 32 + 100;
    gameWindow.create(sf::VideoMode(width, height), "Game Window");
    debugOn = false;
    isPaused = false;
    inLeaderboard = false;
    currentState = PossibleOutcome::ongoing;
    outcomeManager[ongoing] = nullptr;
    outcomeManager[PossibleOutcome::win] = &GameWindow::win;
    outcomeManager[PossibleOutcome::lose] = &GameWindow::lose;
    for (int i = 0; i <= 9; i++) {
        digitManager[i] = [=](sf::Sprite& sprite)->void {
            sprite.setTextureRect(sf::IntRect((i*21),0,21,32));
        };
    }
    digitManager[10] = [](sf::Sprite& sprite)->void {
        sprite.setTextureRect(sf::IntRect((210),0,21,32));
    };
    clockOffset = 0;
    gamePausedLastInterval = false;
}



void GameWindow::win() {
    //set all remaining tiles to flags - check
    //set counter to 0; - check
    //smiley face turns to sunglasses face - check
    //cant interact with tiles - check
    //cant click on debug or pause - check
    //CAN click on leaderboard - check
    //CAN click on sunglasses face to start new game - check

    //save current data to leaderboard
    //save the current person for the * in leaderboard


    debugOn = false;
    smileyButton.updateGraphic("face_win");
    for (auto& row : board.getBoard()) {
        for (auto& tile : row) {
            if (!tile->seeIfFlagged()) tile->toggleFlag();
        }
    }
    leaderboard.addNewRecord(std::make_pair(currentTime, name));

}

void GameWindow::lose() {
    //smiley becomse dead - check
    //all tiles with mines are revealed - check
    //mine is revealed ON TOP of flag
    //cant interact with tiles
    //cant click on debug or pause
    //CAN click on leaderboard
    //CAN click on dead face to start new game

    //save current data to leaderboard


    smileyButton.updateGraphic("face_lose");
    for (auto& row : board.getBoard()) {
        for (auto& tile : row) {
            if (tile->isMine()) tile->revealTile();
        }
    }

}

void GameWindow::reset() {
    inLeaderboard = false;
    debugOn = false;
    isPaused = false;
    clockOffset = 0;
    clock.restart();
    //reset counter
    board.resetBoard();
    currentState = PossibleOutcome::ongoing;
    flagsPlaced = 0;
}

template <std::size_t N>
void GameWindow::displayCustomNumber(std::array<sf::Sprite, N>& arrTextures, std::string stringNum, sf::RenderWindow &window, int realNum) {
    std::vector<int> digits;
    for (size_t i = 0; i < stringNum.length(); i++) {
        char temp = stringNum.at(i);
        digits.push_back(std::atoi(&temp));
    }

    for (size_t i = 0; i < digits.size(); i++) {
        digitManager[digits.at(i)](arrTextures.at(i));
        window.draw(arrTextures.at(i));
    }
    if (realNum < 0) {
        digitManager[10](arrTextures[3]);
        window.draw(arrTextures[3]);
    }
}

void GameWindow::displayCounter(sf::RenderWindow &window) {
    int displayNum = board.getBombCount() - flagsPlaced;
    if (currentState == PossibleOutcome::win) {
        displayNum = 0;
    }
    unsigned int absDisplayNum = abs(displayNum);
    std::string stringNum = std::to_string(absDisplayNum);
    while (stringNum.size()<3) {
        stringNum = "0" + stringNum;
    }
    for (size_t i = 0; i < 4; i++) {
        numbersSprite[i].setTexture(numbersTexture);
        numbersSprite[(i+3)%4].setPosition(12+(21*i), 32 * (board.getConfig().rowCount + 0.5) + 16);
    }
    displayCustomNumber(numbersSprite, stringNum, window, displayNum);
}

std::pair<std::string, std::string> GameWindow::displayClock(sf::RenderWindow &window) {
    if (gamePausedLastInterval) {
        gamePausedLastInterval = false;
        clock.restart();
    }
    sf::Time temp = clock.getElapsedTime();
    if (isPaused || inLeaderboard || currentState != PossibleOutcome::ongoing) {
        clockOffset += temp.asSeconds();
        gamePausedLastInterval = true;
        for (size_t i = 0; i < 2; i++) {
            window.draw(minutesSprite[i]);
            window.draw(secondsSprite[i]);
        }
        int minutes = clockOffset / 60;
        int seconds = clockOffset % 60;
        std::string minutesStr = std::to_string(minutes);
        std::string secondsStr = std::to_string(seconds);
        while (minutesStr.size() < 2) {minutesStr = "0" + minutesStr;}
        while (secondsStr.size() < 2) {secondsStr = "0" + secondsStr;}

        return make_pair(minutesStr, secondsStr);
    }
    int realTime = temp.asSeconds() + clockOffset;
    int minutes = realTime / 60;
    int seconds = realTime % 60;
    std::string minutesStr = std::to_string(minutes);
    std::string secondsStr = std::to_string(seconds);
    while (minutesStr.size() < 2) {minutesStr = "0" + minutesStr;}
    while (secondsStr.size() < 2) {secondsStr = "0" + secondsStr;}
    for (size_t i = 0; i < 2; i++) {
        minutesSprite[i].setTexture(numbersTexture);
        minutesSprite[i].setPosition(board.getConfig().colCount*32-97 + (i*21), 32 * (board.getConfig().rowCount + 0.5) + 16);
        secondsSprite[i].setTexture(numbersTexture);
        secondsSprite[i].setPosition(board.getConfig().colCount*32-54 + (i*21), 32 * (board.getConfig().rowCount + 0.5) + 16);
    }
    displayCustomNumber(secondsSprite, secondsStr, window, seconds);
    displayCustomNumber(minutesSprite, minutesStr, window, minutes);
    return make_pair(minutesStr, secondsStr);
}

void GameWindow::leftClick(int posX, int posY) {
    if (currentState != PossibleOutcome::ongoing) {
        //smily and leaderboard
        bool sentinle = false;
        toggle(smileyButton, sentinle, posX, posY, "face_happy", &GameWindow::reset);
        toggle(leaderboardButton, inLeaderboard, posX, posY, "", nullptr); //change to the LeaderBoard.run()
    }
    else if (isPaused) {
        toggle(pauseButton, isPaused, posX, posY, "pause", nullptr);
    }
    else if (inLeaderboard) {
        return;//detect nothing
    }
    else {
        /*
        * tile (reveal)
            smileyface
            debug
            pause
            leaderboard
         */
        bool sentinle = false;
        toggle(smileyButton, sentinle, posX, posY, "face_happy", &GameWindow::reset);
        toggle(debugButton, debugOn, posX, posY, "", nullptr);
        currentState = board.mousePressed(posX, posY, [&](size_t i, size_t j)->PossibleOutcome {
            if (!board.revealTiles(i, j)) {
                return PossibleOutcome::lose;
            }
            return PossibleOutcome::ongoing;
        });
        toggle(pauseButton, isPaused, posX, posY, "play", nullptr);
        toggle(leaderboardButton, inLeaderboard, posX, posY, "", nullptr);

    }
}

void GameWindow::rightClick(int posX, int posY) {
    if (currentState == ongoing && !isPaused && !inLeaderboard) {
        board.mousePressed(posX, posY, [&](size_t i, size_t j)->PossibleOutcome {
            if (board.getBoard()[i][j]->hasBeenRevealed()) return PossibleOutcome::ongoing;
            board.getBoard()[i][j]->toggleFlag();
            if (board.getBoard()[i][j]->seeIfFlagged()) flagsPlaced++;
            else flagsPlaced--;
            return PossibleOutcome::ongoing;
        });
    }
}


void GameWindow::runLeaderboard() {
    leaderboard.Run(currentState, inLeaderboard);
}



void GameWindow::toggle(Button& button, bool& b, int x, int y, const std::string& fName, void(GameWindow::*action)()) {

    if (button.mousePress(x, y)) {
        b = !b;
        if (!fName.empty()) button.updateGraphic(fName);
        if (action) (this->*action)();
    }
}



void GameWindow::displayButtons(sf::RenderWindow &window) {
    debugButton.displayButton(gameWindow);
    leaderboardButton.displayButton(gameWindow);
    smileyButton.displayButton(gameWindow);
    pauseButton.displayButton(gameWindow);
}


bool GameWindow::Run() {

    //game loop
    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                return true;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                auto mousePosition = sf::Mouse::getPosition(gameWindow);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftClick(mousePosition.x, mousePosition.y);
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    rightClick(mousePosition.x, mousePosition.y);
                }
            }
        }
        gameWindow.clear(sf::Color(sf::Color::White));

        //draw things
        //board
        std::uint32_t count = 0;
        for (size_t i = 0; i < board.getBoard().size(); i++) {
            for (size_t j = 0; j < board.getBoard()[i].size(); j++) {
                board.getBoard()[i][j]->displayTile(gameWindow, debugOn, isPaused, inLeaderboard);
                if (!board.getBoard()[i][j]->isMine() && board.getBoard()[i][j]->hasBeenRevealed()) count++;
            }
        }
        if (count == board.getConfig().colCount * board.getConfig().rowCount - board.getConfig().bombCount) currentState = PossibleOutcome::win;

        //buttons
        displayButtons(gameWindow);

        //timer
        currentTime = displayClock(gameWindow);

        //counter
        displayCounter(gameWindow);

        gameWindow.display();

        if (inLeaderboard) {
            runLeaderboard();
        }

        if (outcomeManager[currentState] != nullptr) {
            (this->*outcomeManager[currentState])();
        }
    }
    return false;
}
