//
// Created by riley on 12/1/2025.
//




#ifndef MINESWEEPERPROJECT_TILE_HPP
#define MINESWEEPERPROJECT_TILE_HPP
#include <memory>
#include <vector>
#include <utility>
#include <string>
#include <SFML/Graphics.hpp>
#include "config.hpp"
#include <unordered_map>



class Tile {
protected:
    std::pair<float, float> positionPair;
    sf::Texture hiddenTexture;
    sf::Sprite hiddenLayer;
    sf::Texture revealedTexture;
    sf::Sprite revealedLayer;
    sf::Texture flagTexture;
    sf::Sprite flag;
    bool isFlagged;
    bool isRevealed;



public:
    Tile(std::size_t x, std::size_t y, const config& config);

    virtual bool displayTile(sf::RenderWindow& window, bool debugOn, bool isPaused) = 0;
    void revealTile();
    virtual bool isMine() = 0;
    void setAdjacentMinesNumber(const short& x);
    void toggleFlag();
    bool seeIfFlagged();
    bool hasBeenRevealed() const;
    bool mousePressed(int x, int y);
    short getNumMines() const;

    virtual ~Tile() = default;

protected:
    short numMines;
};


class RegularTile : public Tile {

public:
    RegularTile(std::size_t x, std::size_t y, const config& config);

    bool displayTile(sf::RenderWindow& window, bool debugOn, bool isPaused) override;
    bool isMine() override;

    ~RegularTile() override = default;

private:
    std::unordered_map<short, sf::Texture> numberTextures;
    sf::Sprite number;


};

class Mine : public Tile {

public:
    Mine(std::size_t x, std::size_t y, const config& config);

    bool displayTile(sf::RenderWindow& window, bool debugOn, bool isPaused) override;
    bool isMine() override;



    ~Mine() override = default;

private:
    sf::Texture mineTexture;
    sf::Sprite mine;

};


#endif //MINESWEEPERPROJECT_TILE_HPP