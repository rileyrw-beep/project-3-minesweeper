//
// Created by riley on 12/1/2025.
//

#ifndef MINESWEEPERPROJECT_BOARD_HPP
#define MINESWEEPERPROJECT_BOARD_HPP
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>
#include <memory>
#include "config.hpp"
#include "PossibleOutcomes.hpp"
#include "Tile.hpp"
#include <functional>
#include <random>

class Board {
public:
    explicit Board(const config& config);
    std::vector<std::vector<std::unique_ptr<Tile>>>& getBoard();
    bool revealTiles(int x, int y);
    void resetBoard();
    PossibleOutcome mousePressed(int x, int y, const std::function<PossibleOutcome(size_t, size_t)>& update);
    const config& getConfig() const;
    void refactorBoard(int y, int x);
    int getBombCount() const;



private:
    std::vector<std::vector<std::unique_ptr<Tile>>> board;
    int bombCount;
    int flagCount;
    config configuration{};
    bool completedFirstMove;
};


#endif //MINESWEEPERPROJECT_BOARD_HPP