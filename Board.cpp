//
// Created by riley on 12/1/2025.
//

#include "Board.hpp"

#include <iostream>

#include "Tile.hpp"


Board::Board(const config &config) {
    configuration = config;
    bombCount = config.bombCount;
    flagCount = 0;
    completedFirstMove = false;
    resetBoard();
}

PossibleOutcome Board::mousePressed(int x, int y, const std::function<PossibleOutcome(size_t, size_t)> &update) {
    PossibleOutcome outcome = ongoing;
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board[i].size(); j++) {
            if (board[i][j]->mousePressed(x, y)) {
                outcome = update(i, j);
            }
        }
    }
    return outcome;
}

void Board::refactorBoard(int y, int x) {
    std::random_device rand;
    std::mt19937 mt(rand());
    std::uniform_int_distribution<int> dist(0, 100); // Numbers between 1 and 100 (inclusive)

    int numMinesToAddBack = 0;
    std::cout << "refactoring" << std::endl;
    if (board[y][x]->isMine()) {
        board[y][x].reset(new RegularTile(x, y, configuration));
        numMinesToAddBack++;
    }
    if ((x > 0 && y > 0) && board[y - 1][x - 1]->isMine()) {
        board[y - 1][x - 1].reset(new RegularTile(x-1, y-1, configuration));
        numMinesToAddBack++;
    }
    if (x + 1 < configuration.colCount && y > 0 && board[y - 1][x + 1]->isMine()) {
        board[y - 1][x + 1].reset(new RegularTile(x+1, y-1, configuration));
        numMinesToAddBack++;
    }
    if (x > 0 && y + 1 < configuration.rowCount && board[y + 1][x - 1]->isMine()) {
        board[y + 1][x - 1].reset(new RegularTile(x-1, y+1, configuration));
        numMinesToAddBack++;
    }
    if (x + 1 < configuration.colCount && y + 1 < configuration.rowCount && board[y + 1][x + 1]->isMine()) {
        board[y + 1][x + 1].reset(new RegularTile(x+1, y+1, configuration));
        numMinesToAddBack++;
    }
    if (y > 0 && board[y - 1][x]->isMine()) {
        board[y - 1][x].reset(new RegularTile(x, y-1, configuration));
        numMinesToAddBack++;
    }
    if (y + 1 < configuration.rowCount && board[y + 1][x]->isMine()) {
        board[y + 1][x].reset(new RegularTile(x, y+1, configuration));
        numMinesToAddBack++;
    }
    if (x > 0 && board[y][x - 1]->isMine()) {
        board[y][x - 1].reset(new RegularTile(x-1, y, configuration));
        numMinesToAddBack++;
    }
    if (x + 1 < configuration.colCount && board[y][x + 1]->isMine()) {
        board[y][x + 1].reset(new RegularTile(x+1, y, configuration));
        numMinesToAddBack++;
    }

    bool minesAdded = false;
    int count = 0;
    while (true) {
        for (size_t i = 0; i < board.size(); i++) {
            for (size_t j = 0; j < board[i].size(); j++) {
                if (!board[i][j]->isMine()) {
                    if (count==numMinesToAddBack) {
                        minesAdded = true;
                        break;
                    }
                    if ((i == y && j == x) || (i == y+1 && j == x+1) || (i == y-1 && j == x-1) || (i == y+1 && j == x-1) || (i == y-1 && j == x+1) || (i == y+1 && j == x) || (i == y-1 && j == x) || (i == y && j == x+1) || (i == y && j == x-1)) {
                        continue;
                    }
                    if (dist(mt) <= 20) {
                        board[i][j].reset(new Mine(j, i, configuration));
                        count++;
                    }
                }
            }
            if (minesAdded) {
                break;
            }
        }
        if (minesAdded) break;
    }

    for (size_t y = 0; y < board.size(); y++) {
        for (size_t x = 0; x < board[y].size(); x++) {
            if (!board[y][x]->isMine()) {
                short adjacentMineCount = 0;
                if ((x > 0 && y > 0) && board[y - 1][x - 1]->isMine()) adjacentMineCount++; //not good
                if (x + 1 < configuration.colCount && y > 0 && board[y - 1][x + 1]->isMine()) adjacentMineCount++;
                if (x > 0 && y + 1 < configuration.rowCount && board[y + 1][x - 1]->isMine()) adjacentMineCount++;
                if (x + 1 < configuration.colCount && y + 1 < configuration.rowCount && board[y + 1][x + 1]->isMine()) adjacentMineCount++;
                if (y > 0 && board[y - 1][x]->isMine()) adjacentMineCount++;
                if (y + 1 < configuration.rowCount && board[y + 1][x]->isMine()) adjacentMineCount++;
                if (x > 0 && board[y][x - 1]->isMine()) adjacentMineCount++;
                if (x + 1 < configuration.colCount && board[y][x + 1]->isMine()) adjacentMineCount++;
                board[y][x]->setAdjacentMinesNumber(adjacentMineCount);
            }
        }
    }
}

int Board::getBombCount() const {
    return bombCount;
}


bool Board::revealTiles(int y, int x) {
    if (y < 0 || x < 0 || y >= configuration.rowCount || x >= configuration.colCount) return true;
    if (board[y][x]->hasBeenRevealed() || board[y][x]->seeIfFlagged()) return true;
    if (!completedFirstMove) {
        completedFirstMove = true;
        if (board[y][x]->isMine() || board[y][x]->getNumMines() > 0) refactorBoard(y, x);
    }



    if (board[y][x]->isMine()) {
        std::cout << "boom" << std::endl;
        return false;
    }
    board[y][x]->revealTile();

    if (board[y][x]->getNumMines() > 0) {
        return true;
    }
    revealTiles(y + 1, x + 1);
    revealTiles(y - 1, x - 1);
    revealTiles(y + 1, x - 1);
    revealTiles(y - 1, x + 1);
    revealTiles(y + 1, x);
    revealTiles(y - 1, x);
    revealTiles(y, x + 1);
    revealTiles(y, x - 1);
    return true;
}

const config &Board::getConfig() const {
    return configuration;
}



void Board::resetBoard() {
    board.clear();
    completedFirstMove = false;

    std::random_device rand;
    std::mt19937 mt(rand());
    std::uniform_int_distribution<int> dist(0, 100); // Numbers between 1 and 100 (inclusive)

    std::size_t count = 0;
    for (std::size_t i = 0; i < configuration.rowCount; i++) {
        std::vector<std::unique_ptr<Tile> > row;
        for (std::size_t j = 0; j < configuration.colCount; j++) {
            if (dist(mt) <= 20) {
                row.emplace_back(new Mine(j, i, configuration));
                count++;
            } else {
                row.emplace_back(new RegularTile(j, i, configuration));
            }
        }
        board.emplace_back(std::move(row));
    }



    bool countReached = false;
    while (count != bombCount) {
        if (count < bombCount) {
            for (size_t i = 0; i < board.size(); i++) {
                for (size_t j = 0; j < board[i].size(); j++) {
                    if (count == bombCount) {
                        countReached = true;
                        break;
                    }
                    if (!board[i][j]->isMine()) {
                        if (dist(mt) <= 20) {
                            board[i][j].reset(new Mine(j, i, configuration));
                            count++;
                        }
                    }
                }
                if (countReached) {
                    break;
                }
            }
            if (countReached) break;
        } else {
            for (size_t i = 0; i < board.size(); i++) {
                for (size_t j = 0; j < board[i].size(); j++) {
                    if (count == bombCount) {
                        countReached = true;
                        break;
                    }
                    if (board[i][j]->isMine()) {
                        if (dist(mt) <= 20) {
                            board[i][j].reset(new RegularTile(j, i, configuration));
                            count--;
                        }
                    }
                }
                if (countReached) {break;}
            }
        }
        if (countReached) break;
    }



    //we need to figure out the number of bombs that every non bomb
    for (size_t y = 0; y < board.size(); y++) {
        for (size_t x = 0; x < board[y].size(); x++) {
            if (!board[y][x]->isMine()) {
                short adjacentMineCount = 0;
                if ((x > 0 && y > 0) && board[y - 1][x - 1]->isMine()) adjacentMineCount++; //not good
                if (x + 1 < configuration.colCount && y > 0 && board[y - 1][x + 1]->isMine()) adjacentMineCount++;
                if (x > 0 && y + 1 < configuration.rowCount && board[y + 1][x - 1]->isMine()) adjacentMineCount++;
                if (x + 1 < configuration.colCount && y + 1 < configuration.rowCount && board[y + 1][x + 1]->isMine()) adjacentMineCount++;
                if (y > 0 && board[y - 1][x]->isMine()) adjacentMineCount++;
                if (y + 1 < configuration.rowCount && board[y + 1][x]->isMine()) adjacentMineCount++;
                if (x > 0 && board[y][x - 1]->isMine()) adjacentMineCount++;
                if (x + 1 < configuration.colCount && board[y][x + 1]->isMine()) adjacentMineCount++;
                board[y][x]->setAdjacentMinesNumber(adjacentMineCount);
            }
        }
    }
}


std::vector<std::vector<std::unique_ptr<Tile> > > &Board::getBoard() {
    return board;
}
