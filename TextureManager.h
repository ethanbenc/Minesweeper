//
// Created by ethan on 12/4/2022.
//

#ifndef MINESWEEPER_TEXTUREMANAGER_H
#define MINESWEEPER_TEXTUREMANAGER_H
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
using namespace std;

class TextureManager {
    static unordered_map<string, sf::Texture> textures;
    static void loadTexture(string textureName);
public:
    static sf::Texture& getTexture(string textureName);
    static void Clear();
};
#endif //MINESWEEPER_TEXTUREMANAGER_H
