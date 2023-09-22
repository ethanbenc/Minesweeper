//
// Created by ethan on 12/4/2022.
//

#include "Tile.h"

Tile::Tile() {}

Tile::Tile(unsigned int y_coord, unsigned int x_coord) {
    y = y_coord;
    x = x_coord;
    isRevealed = false;
    hasMine = false;
    isFlagged = false;

}

bool Tile::getFlagState() {
    return isFlagged;
}
bool Tile::getMineState() {
    return hasMine;
}
bool Tile::getRevealedState() {
    return isRevealed;
}
unsigned int Tile::getXCoord() {
    return x;
}

unsigned int Tile::getYCoord() {
    return y;
}

unsigned int Tile::getNumMines() {
    return numMines;
}

void Tile::countNumMines(vector<vector<Tile>>& tileVector, int& y, int& x) {
    unsigned int count = 0;
    if(!hasMine) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                // tests for out of bounds, ignores current tile
                if((i == 0 && j == 0) || (i + y < 0 || j + x < 0 || tileVector.size() <= i + y || tileVector[0].size() <= j + x)) {
                    continue;
                }

                // get tile and find if it has mine, if it does increase count
                if (tileVector[i+y][j+x].getMineState())
                    count++;

            }
        }
    }
    numMines = count;
}
bool Tile::openNearbyTiles(vector<vector<Tile>>& tileVector, int& y, int& x) {
    if (!isRevealed && !getFlagState()) {
        tileVector[y][x].setRevealed();

        if (!hasMine && numMines == 0) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    // tests for out of bounds, ignores current tile
                    if((i == 0 && j == 0) || (i + y < 0 || j + x < 0 || tileVector.size() <= i + y || tileVector[0].size() <= j + x)) {
                        continue;
                    }
                    // reveals nearby cells and have them call openNearbyTiles to see if there are any other tiles that have no surrounding mines
                    int tempY = i + y;
                    int tempX = j + x;
                    tileVector[i+y][j+x].openNearbyTiles(tileVector, tempY, tempX);
                }
            }
        }
        return hasMine;     // if location has a mine, don't open tile, end recursion
    }
    return 0;
}

void Tile::flag() {
    isFlagged = !isFlagged;     // switches from true to false and vice versa
}

void Tile::clearFlag() {
    isFlagged = false;
}

void Tile::setMine() {
    hasMine = true;
}

void Tile::setRevealed() {
    isRevealed = true;
}


