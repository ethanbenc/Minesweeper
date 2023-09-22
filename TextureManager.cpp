//
// Created by ethan on 12/4/2022.
//
#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

// will be called by getTexture, no need to call
void TextureManager::loadTexture(string fileName) {
    string path = "images/";
    path += fileName + ".png";

    textures[fileName].loadFromFile(path);
}

// creates texture when called, can be called statically
sf::Texture& TextureManager::getTexture(std::string textureName) {
    if(textures.find(textureName) == textures.end())
    {
        loadTexture(textureName);
    }
    return textures[textureName];
}
// clears all textures at the end to clear memory
void TextureManager::Clear() {
    textures.clear();
}
