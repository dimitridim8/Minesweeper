#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Texture.h"
#include <iostream>
using namespace std;

class Tile {

private:
    int row;
    int column;
    bool hasMine;
    bool isHidden;
    bool isFlagged;
    bool gameLost;
    int nearbyMines;

    sf::Sprite tileHidden;
    sf::Sprite tileRevealed;
    sf::Sprite mine;
    sf::Sprite flag;

public:

    vector<Tile*> neighbors;
    vector<sf::Sprite> numbers;

    Tile(){
        isHidden = true;
        hasMine = false;
        isFlagged = false;
        gameLost = false;

        tileHidden.setTexture(Texture::getTexture("tile_hidden"));
        tileRevealed.setTexture(Texture::getTexture("tile_revealed"));

        flag.setTexture(Texture::getTexture("flag"));
        mine.setTexture(Texture::getTexture("mine"));

        for (int i = 1; i <= 8; i++) {
            string numFile = "number_" + to_string(i);
            sf::Sprite num(Texture::getTexture(numFile));
            numbers.push_back(num);
        }
    }

    bool getIsFlagged(){
        return isFlagged;
    }
    bool getIsHidden(){
        return isHidden;
    }
    bool getHasMine(){
        return hasMine;
    }
    bool getNoMine(){
        hasMine = false;
        return hasMine;
    }
    int getNearbyMines(){
        return nearbyMines;
    }
    int getRow(){
        return row;
    }
    int getColumn(){
        return column;
    }
    bool hasNumber() const {
        return !isHidden && !hasMine && nearbyMines > 0;
    }

    sf::Sprite getTile(){
        if (!isHidden && !hasMine && nearbyMines > 0) {
            return numbers[nearbyMines - 1];
        }

        if (!isHidden && hasMine) {
            //cout << "We have a mine" << endl;
            return mine;
        }

        if (isHidden && isFlagged) {
            return flag;
        }

        if (gameLost && hasMine){
            return mine;
        }

        return isHidden ? tileHidden : tileRevealed;
    }
    sf::Sprite getMine(){
        return mine;
    }
    sf::Sprite getFlag(){
        return flag;
    }
    sf::Sprite getTileRevealed(){
        return tileRevealed;
    }
    sf::FloatRect getPosition(){
        return tileHidden.getGlobalBounds();
    }

    vector<Tile*> getNeighbors(){
        return neighbors;
    }

    sf::Sprite getNumberSprite() const {
        if (hasNumber()) {
            return numbers[nearbyMines - 1];
        }
    }

    void setNeighbors(vector<Tile*> neighbors){
        this->neighbors = neighbors;
    }

    void setMines(int mines){
        nearbyMines = mines;
    }

    void setColumn(int c){
        column = c;
    }
    void setRow(int r){
        row = r;
    }
    void revealTile(){
        isHidden = false;
    }
    void unRevealTile(){
        isHidden = true;
    }
    void unRevealMine(){
        hasMine = false;
    }
    void revealFlag(){
        isFlagged = true;
    }
    void unRevealFlag(){
        isFlagged = false;
    }
    void createMine(){
        hasMine = true;
    }
    void setPosition(float x, float y){
        tileHidden.setPosition(x, y);
        tileRevealed.setPosition(x, y);
        flag.setPosition(x, y);
        mine.setPosition(x, y);

        for (int i = 0; i < 8; i++) {
            numbers[i].setPosition(x, y);
        }
    }

};