//
// Created by ethan on 12/4/2022.
//

#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H
#pragma once
#include <vector>
using namespace std;

class Tile {
    bool isFlagged;
    bool hasMine;
    bool isRevealed;

    unsigned int numMines;
    unsigned int x;
    unsigned int y;
public:
    Tile();
    Tile(unsigned int x_coord, unsigned int y_coord);

    bool getFlagState();
    bool getMineState();
    bool getRevealedState();
    unsigned int getXCoord();
    unsigned int getYCoord();

    unsigned int getNumMines();

    void countNumMines(vector<vector<Tile>>& tileVector, int& x, int& y);
    bool openNearbyTiles(vector<vector<Tile>>& tileVector, int& x, int& y);
    void flag();
    void clearFlag();
    void setMine();
    void setRevealed();

};


#endif //MINESWEEPER_TILE_H
