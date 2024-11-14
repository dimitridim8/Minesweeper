#include "Texture.h"
#include <string>
using namespace std;

unordered_map<string, sf::Texture> Texture::textures;

sf::Texture& Texture::getTexture(string name) {
    if (textures.find(name) == textures.end()) {
        loadTexture(name);
    }
    return textures[name];
}

void Texture::clear() {
    textures.clear();
}

void Texture::loadFromFile(const string& name, const string& filePath) {
    if (textures.find(name) != textures.end()) {
        clear();
    }
    sf::Texture texture;
    if (texture.loadFromFile(filePath)) {
        textures[name] = texture;
    }
}

void Texture::loadTexture(string name) {
    string path = "images/";
    path += name + ".png";
    textures[name].loadFromFile(path);
}
