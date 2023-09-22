//
// Created by ethan on 12/4/2022.
//

#include "Random.h"
#include <ctime>
std::mt19937 Random::random(time(0));

// creates a random int between min and max, can be called statically
int Random::Int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(random);
}

// unused, but creates a random float value if needed, can be called statically
float Random::Float(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(random);
}