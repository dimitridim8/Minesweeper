#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>
using namespace std;

class Texture{
private:
    static unordered_map<string, sf::Texture> textures;
    static void loadTexture(string name);

public:
    static sf::Texture& getTexture(string name);
    static void clear();
    static void loadFromFile(const string& name, const string& filePath);
};
