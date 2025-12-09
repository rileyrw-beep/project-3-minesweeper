//
// Created by riley on 12/1/2025.
//

#include "Tile.hpp"

#include <iostream>

Tile::Tile(std::size_t x, std::size_t y, const config &config) {
    //creates adjacentTiles array based on x and y [and it knows when it will fly off the board]

    //they should start out hidden and not flagged
    isFlagged = false;
    isRevealed = false;

    if (!hiddenTexture.loadFromFile("files/images/tile_hidden.png")) { std::exit(1); }
    if (!revealedTexture.loadFromFile("files/images/tile_revealed.png")) { std::exit(1); }
    if (!flagTexture.loadFromFile("files/images/flag.png")) { std::exit(1); }
    hiddenLayer.setTexture(hiddenTexture);
    revealedLayer.setTexture(revealedTexture);
    flag.setTexture(flagTexture);

    positionPair = {static_cast<float>(x) * 32.0f, static_cast<float>(y) * 32.0f};

    //hiddenLayer.setTexture(hiddenTexture);
    hiddenLayer.setPosition(positionPair.first, positionPair.second);
    //revealedLayer.setTexture(revealedTexture);
    revealedLayer.setPosition(positionPair.first, positionPair.second);
    //flag.setTexture(flagTexture);
    flag.setPosition(positionPair.first, positionPair.second);

    numMines = -1;


}
void Tile::setAdjacentMinesNumber(const short& x) {
    numMines = x;
}

void Tile::toggleFlag() {
    isFlagged = !isFlagged;
}

bool Tile::seeIfFlagged() {
    return isFlagged;
}

bool Tile::mousePressed(int x, int y) {
    return hiddenLayer.getGlobalBounds().contains(x, y);
}

short Tile::getNumMines() const {
    return numMines;
}

bool Tile::hasBeenRevealed() const {
    return isRevealed;
}

void Tile::revealTile() {
    isRevealed = true;
}

//reg tile
RegularTile::RegularTile(std::size_t x, std::size_t y, const config &config) : Tile(x, y, config) {
    for (short i = 1; i <= 8; i++) {
        std::string fileName = "files/images/number_" + std::to_string(i) + ".png";
        numberTextures[i].loadFromFile(fileName);
    }
}

bool RegularTile::isMine() {
    return false;
}


bool RegularTile::displayTile(sf::RenderWindow& window, bool debugOn, bool isPaused, bool inLeaderboard) {
    window.draw(revealedLayer);

    if (isPaused || inLeaderboard) {
        return true;
    }

    if (isRevealed) {
        if (numMines > 0) {
            number.setTexture(numberTextures[numMines]);
            number.setPosition(positionPair.first, positionPair.second);
            window.draw(number);
        }
    }
    else {
        window.draw(hiddenLayer);
        if (isFlagged) {
            window.draw(flag);
        }
    }
    return true;
}

//mine
Mine::Mine(std::size_t x, std::size_t y, const config &config) : Tile(x, y, config) {
    if(!mineTexture.loadFromFile("files/images/mine.png")) {std::exit(1);};
    mine.setTexture(mineTexture);
    mine.setPosition(positionPair.first, positionPair.second);
}

bool Mine::isMine() {
    return true;
}


bool Mine::displayTile(sf::RenderWindow& window, bool debugOn, bool isPaused, bool inLeaderboard) {
    window.draw(revealedLayer);
    if (isPaused || inLeaderboard) {
        return true;
    }
    if (!isRevealed) {
        window.draw(hiddenLayer);
        if (isFlagged && !debugOn) {
            window.draw(flag);
        }
        if (debugOn) {
            window.draw(mine);
        }
        return true;
    }
    if (isFlagged) window.draw(flag);
    window.draw(mine);
    return false;



}


