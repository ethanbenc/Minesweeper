//
// Created by ethan on 12/4/2022.
//

#ifndef MINESWEEPER_RANDOM_H
#define MINESWEEPER_RANDOM_H
#pragma once
#include <random>

class Random {
    static std::mt19937 random;
public:
    static int Int(int min, int max);
    static float Float(float min, float max);
};


#endif //MINESWEEPER_RANDOM_H
