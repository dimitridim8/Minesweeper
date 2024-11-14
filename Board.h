#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <chrono>
using namespace std;

class Board {
private:
    int columns;
    int rows;
    int mines;
    int totalFlagged;
    int testMines;
    int tilesRevealed;
    int totalTilesWithoutMines;
    int mineCount;

    int width;
    int height;

    int timeElapsed;
    int min;
    int min2;
    int sec;
    int sec2;
    double endTime;

    bool gameWon;
    bool gameLost;
    bool isDebug;
    bool isTestBoard;
    bool isNegativeMines;
    bool isPlaying;
    bool isPaused;
    bool timerStarted;
    bool leaderboardOpen;
    bool debugOff;

    sf::Sprite smiley;
    sf::Sprite mineDigits;
    sf::Sprite faceWin;
    sf::Sprite faceLose;
    sf::Sprite debug;
    sf::Sprite test1;
    sf::Sprite test2;
    sf::Sprite test3;
    sf::Sprite pause;
    sf::Sprite play;
    sf::Sprite leaderboard;

    sf::Sprite firstDigitSprite;
    sf::Sprite secondDigitSprite;
    sf::Sprite thirdDigitSprite;
    sf::Sprite negative;
    sf::Sprite timerMin;
    sf::Sprite timerMin2;
    sf::Sprite timerSec;
    sf::Sprite timerSec2;
    sf::Clock clock;
    sf::IntRect digitRect;

public:
    vector<vector<Tile> > tiles;
    vector<vector<int> > intTiles;
    vector<sf::Sprite> digits;

    Board() {

        std::ifstream configFile("board_config.cfg");
        if (configFile.is_open()) {
            std::string line;
            int lineCounter = 0;
            while (std::getline(configFile, line) && lineCounter < 3) {
                std::istringstream iss(line);
                std::string field, value;
                iss >> value;
                switch (lineCounter) {
                    case 0:
                        columns = std::stoi(value);
                        break;
                    case 1:
                        rows = std::stoi(value);
                        break;
                    case 2:
                        mines = std::stoi(value);
                        break;
                }
                lineCounter++;
            }
            configFile.close();
        }


        //columns = 25;
        //rows = 16;
        //mines = 50;
        tiles.resize(rows);
        for (auto &x: tiles) {
            x.resize(columns);
        }

        gameWon = false;
        gameLost = false;
        isDebug = false;
        isTestBoard = false;
        isNegativeMines = false;
        isPlaying = true;
        isPaused = false;
        timerStarted = true;
        leaderboardOpen = false;
        debugOff = true;

        totalFlagged = 0;

        width = columns * 32;
        height = (rows * 32) + 100;

        timeElapsed = 0;
        min = 0;
        sec = 0;
        min2 = 0;
        sec2 = 0;

        smiley.setTexture(Texture::getTexture("face_happy"));
        faceWin.setTexture(Texture::getTexture("face_win"));
        faceLose.setTexture(Texture::getTexture("face_lose"));
        debug.setTexture(Texture::getTexture("debug"));
        //test1.setTexture(Texture::getTexture("test_1"));
        //test2.setTexture(Texture::getTexture("test_2"));
        //test3.setTexture(Texture::getTexture("test_3"));
        mineDigits.setTexture(Texture::getTexture("digits"));
        timerMin.setTexture(Texture::getTexture("digits"));
        timerSec.setTexture(Texture::getTexture("digits"));
        timerMin2.setTexture(Texture::getTexture("digits"));
        timerSec2.setTexture(Texture::getTexture("digits"));
        play.setTexture(Texture::getTexture("play"));
        pause.setTexture(Texture::getTexture("pause"));
        leaderboard.setTexture(Texture::getTexture("leaderboard"));
        digitRect = sf::IntRect(0, 0, 21, 32);

        firstDigitSprite.setPosition(33, 32 * ((rows) + 0.5f) + 16);
        secondDigitSprite.setPosition(54, 32 * ((rows) + 0.5f) + 16);
        thirdDigitSprite.setPosition(75, 32 * ((rows) + 0.5f) + 16);
        faceWin.setPosition(-100, -100);
        faceLose.setPosition(-100, -100);
        smiley.setPosition(((columns / 2.0) * 32) - 32, 32 * (rows + 0.5f));
        debug.setPosition((columns * 32) - 304, 32 * (rows + 0.5f));
        leaderboard.setPosition((columns * 32) - 176, 32 * (rows + 0.5f));
        timerMin.setPosition(((columns) * 32) - 77, 32 * ((rows) + 0.5f) + 16);
        timerSec.setPosition(((columns) * 32) - 34, 32 * ((rows) + 0.5f) + 16);
        timerMin2.setPosition(((columns) * 32) - 97, 32 * ((rows) + 0.5f) + 16);
        timerSec2.setPosition(((columns) * 32) - 54, 32 * ((rows) + 0.5f) + 16);


        totalTilesWithoutMines = (rows * columns) - mines;

    }

    int getColumns(){
        return columns;
    }
    int getRows(){
        return rows;
    }
    int getMines(){
        return mines;
    }
    int getWidth(){
        return width;
    }
    int getHeight(){
        return height;
    }
    bool getGameWon(){
        return gameWon;
    }
    bool getGameLost(){
        return gameLost;
    }

    bool isValidBoardPos(int row, int col){
        return (row >= 0 && row < rows && col >= 0 && col < columns);
    }

    void setDimensions(){
        ifstream file("boards/config.cfg");
        string tempNumber;

        int tempColumns;
        int tempRows;
        int tempMines;

        if (file.is_open()) {
            getline(file, tempNumber);
            tempColumns = stoi(tempNumber);

            getline(file, tempNumber);
            tempRows = stoi(tempNumber);

            getline(file, tempNumber);
            tempMines = stoi(tempNumber);

            columns = tempColumns;
            rows = tempRows;
            mines = tempMines;
        }
        else {
            //cout << "file not opened...";
        }
    }

    void drawBoard(sf::RenderWindow& window) {
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < columns; k++) {
                tiles[i][k].setPosition(k * 32, i * 32);
                tiles[i][k].setColumn(i);
                tiles[i][k].setRow(k);

                if (!tiles[i][k].getIsHidden() && (tiles[i][k].getNearbyMines() > 0 || tiles[i][k].getHasMine())) {
                    window.draw(tiles[i][k].getTileRevealed());
                    if (tiles[i][k].hasNumber() && isPlaying) {
                        sf::Sprite number = tiles[i][k].getNumberSprite();
                        //number.setPosition(tiles[i][k].getX(), tiles[i][k].getY());
                        window.draw(number);
                    }
                }
                else {
                    window.draw(tiles[i][k].getTile());
                }

                if (isDebug && tiles[i][k].getHasMine()){
                    sf::Sprite mineSprite;
                    mineSprite.setTexture(Texture::getTexture("mine"));
                    mineSprite.setPosition(k * 32, i * 32);
                    window.draw(mineSprite);
                }
                if(isDebug && tiles[i][k].getIsFlagged() && !tiles[i][k].getHasMine()){
                    sf::Sprite flag;
                    flag.setTexture(Texture::getTexture("flag"));
                    flag.setPosition(k * 32, i * 32);
                    window.draw(flag);
                }
                if (!isDebug && tiles[i][k].getIsFlagged() && isPlaying){
                    sf::Sprite tile;
                    tile.setTexture(Texture::getTexture("tile_hidden"));
                    tile.setPosition(k * 32, i * 32);
                    window.draw(tile);
                    sf::Sprite flag;
                    flag.setTexture(Texture::getTexture("flag"));
                    flag.setPosition(k * 32, i * 32);
                    window.draw(flag);
                }

                if (gameLost && tiles[i][k].getHasMine()) {
                    sf::Sprite mineSprite;
                    mineSprite.setTexture(Texture::getTexture("mine"));
                    mineSprite.setPosition(k * 32, i * 32);
                    window.draw(mineSprite);
                }
                else if (gameWon && tiles[i][k].getHasMine() && !tiles[i][k].getIsFlagged()) {
                    window.draw(tiles[i][k].getFlag());
                }
            }
        }

        if (gameLost) {
            for (int i = 0; i < rows; i++) {
                for (int k = 0; k < columns; k++) {
                    if (intTiles[i][k] == 1) {
                        sf::Sprite mine;
                        mine.setTexture(Texture::getTexture("mine"));
                        mine.setPosition(k * 32, i * 32);
                        window.draw(mine);
                    }
                }
            }
        }

        sf::RectangleShape rectangle(sf::Vector2f(window.getSize().x, 50));
        rectangle.setFillColor(sf::Color::White);
        rectangle.setPosition(0, window.getSize().y - 50);
        window.draw(rectangle);

        sf::RectangleShape rectangle2(sf::Vector2f(window.getSize().x, 50));
        rectangle2.setFillColor(sf::Color::White);
        rectangle2.setPosition(0, window.getSize().y - 100);
        window.draw(rectangle2);
    }

    void drawBoardSprites(sf::RenderWindow& window){
        if (!gameWon && !gameLost) {
            window.draw(smiley);
        }
        else if (gameLost) {
            window.draw(faceLose);
        }
        else {
            window.draw(faceWin);
        }
        //window.draw(test1);
        //window.draw(test2);
        //window.draw(test3);
        window.draw(debug);
        window.draw(pause);
        window.draw(play);
        window.draw(leaderboard);
        window.draw(timerMin2);
        window.draw(timerMin);
        window.draw(timerSec2);
        window.draw(timerSec);
        updateMineCounter(window);
    }

    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point pauseTime;
    std::chrono::high_resolution_clock::time_point resumeTime;

    void startTimer() {
        startTime = std::chrono::high_resolution_clock::now();
        timerStarted = true;
    }

    double getElapsedTime() {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - startTime);
        return elapsed.count();
    }

    void updateTimer() {
        double elapsed;
        if (!gameWon && !gameLost) {
            if (!isPaused) {
                elapsed = getElapsedTime();
            } else {
                elapsed = chrono::duration_cast<chrono::duration<double>>(pauseTime - startTime).count();
            }

            int totalSeconds = static_cast<int>(elapsed);
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;

            timerMin2.setTextureRect(sf::IntRect((minutes / 10) * 21, 0, 21, 32));
            timerMin.setTextureRect(sf::IntRect((minutes % 10) * 21, 0, 21, 32));
            timerSec2.setTextureRect(sf::IntRect((seconds / 10) * 21, 0, 21, 32));
            timerSec.setTextureRect(sf::IntRect((seconds % 10) * 21, 0, 21, 32));
        }
    }

    void handlePlayPause(sf::Event event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (play.getGlobalBounds().contains(mousePos)) {
                isPlaying = true;
                isPaused = false;
                if (!timerStarted) {
                    startTimer();
                    timerStarted = true;
                }
                else {
                    auto now = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - pauseTime);
                    startTime += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(elapsed);
                }
                resumeTime = std::chrono::steady_clock::now();
            } else if (pause.getGlobalBounds().contains(mousePos)) {
                isPlaying = false;
                isPaused = true;
                pauseTime = std::chrono::steady_clock::now();
            }
        }

        if (isPlaying) {
            pause.setPosition((columns * 32) - 240, 32 * (rows + 0.5f));
            play.setPosition(-100, -100);
        }
        else if (isPaused) {
            play.setPosition((columns * 32) - 240, 32 * (rows + 0.5f));
            pause.setPosition(-100, -100);
            for (int i = 0; i < rows; i++) {
                for (int k = 0; k < columns; k++) {
                    if(tiles[i][k].getIsHidden()) {
                        sf::Sprite tile;
                        tile.setTexture(Texture::getTexture("tile_revealed"));
                        tile.setPosition(k * 32, i * 32);
                        window.draw(tile);
                    }
                }
            }
        }
    }

    void checkTileClicked(sf::RenderWindow& window){
        if (!gameLost || gameWon) {
            auto mousePos = sf::Mouse::getPosition(window);
            auto actualMousePos = window.mapPixelToCoords(mousePos);
            for (int i = 0; i < rows; i++) {
                for (int k = 0; k < columns; k++) {
                    if (tiles[i][k].getPosition().contains(actualMousePos)) {
                        if(isPlaying) {
                            if (gameWon) {
                                endGame(window);
                                break;
                            }

                            if (tiles[i][k].getHasMine()) {
                                gameLost = true;
                                endGame(window);
                            }

                            recursiveClick(&tiles[i][k]);
                        }

                    }
                }
            }
        }
    }

    void flagATile(sf::RenderWindow& window){
        auto mousePos = sf::Mouse::getPosition(window);
        //auto actualMousePos = window.mapPixelToCoords(mousePos);

        if (!gameWon && !gameLost && !isDebug) {
            for (int i = 0; i < rows; i++) {
                for (int k = 0; k < columns; k++) {
                    if (tiles[i][k].getPosition().contains(mousePos.x, mousePos.y)) {
                        if (!tiles[i][k].getIsFlagged()) {
                            tiles[i][k].revealFlag();
                            totalFlagged++;
                        }
                        else {
                            tiles[i][k].unRevealFlag();
                            totalFlagged--;
                        }
                    }
                }
            }
        }
    }

    void addEachNeighbor(){
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                vector<Tile*> nbs;
                if (isValidBoardPos(r - 1, c + 1)) {
                    Tile* temp(&tiles[r - 1][c + 1]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }

                if (isValidBoardPos(r + 1, c + 1)) {
                    Tile* temp(&tiles[r + 1][c + 1]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }

                if (isValidBoardPos(r, c + 1)) {
                    Tile* temp(&tiles[r][c + 1]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }

                if (isValidBoardPos(r - 1, c - 1)) {
                    Tile* temp(&tiles[r - 1][c - 1]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }

                if (isValidBoardPos(r + 1, c - 1)) {
                    Tile* temp(&tiles[r + 1][c - 1]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }

                if (isValidBoardPos(r, c - 1)) {
                    Tile* temp(&tiles[r][c - 1]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }

                if (isValidBoardPos(r - 1, c)) {
                    Tile* temp(&tiles[r - 1][c]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }

                if (isValidBoardPos(r + 1, c)) {
                    Tile* temp(&tiles[r + 1][c]);
                    nbs.push_back(temp);
                }
                else {
                    nbs.push_back(nullptr);
                }
                tiles[r][c].setNeighbors(nbs);
            }
        }
    }

    void createRandomIntBoard(){
        isTestBoard = false;
        intTiles.clear();
        vector<int> vals(columns * rows);
        generate(vals.begin(), vals.end(), [val = 0]() mutable { return val++; });
        shuffle(vals.begin(), vals.end(), mt19937{ random_device{}() });
        vals.resize(mines);

        int c = 0;
        for (int i = 0; i < rows; i++) {
            vector<int> tempArr;
            for (int k = 0; k < columns; k++) {
                if (find(vals.begin(), vals.end(), c) != vals.end()) {
                    tempArr.push_back(1);
                }
                else {
                    tempArr.push_back(0);
                }
                c++;
            }
            intTiles.push_back(tempArr);
            tempArr.clear();
        }

        totalTilesWithoutMines = (columns * rows) - mines;
    }

    void createBoardBasedOffInts(){
        tiles.clear();
        for (int i = 0; i < rows; i++) {
            vector<Tile> tempTiles;
            for (int k = 0; k < columns; k++) {
                Tile t;
                if (intTiles[i][k] == 1) {
                    t.createMine();
                }
                tempTiles.push_back(t);
            }
            tiles.push_back(tempTiles);
            tempTiles.clear();
        }

        gameLost = false;
        gameWon = false;
        isNegativeMines = false;
        tilesRevealed = 0;
        totalFlagged = 0;
        addEachNeighbor();
        countSurroundingMines();
    }

    void readInTestBoard(int n){
        intTiles.clear();
        string filename = "boards/testboard" + to_string(n) + ".brd";

        fstream file(filename);
        string lineFromFile;

        int temp = 0;

        if (file.is_open()) {
            while (getline(file, lineFromFile)) {
                vector<int> tempInts;
                for (int i = 0; i < lineFromFile.size(); i++) {
                    tempInts.push_back(((int)lineFromFile[i]) - 48);

                    if (((int)lineFromFile[i]) - 48 == 1) {
                        temp++;
                    }

                }

                intTiles.push_back(tempInts);
                tempInts.clear();
            }

            columns = intTiles[0].size();
            rows = intTiles.size();
        }
        else {
            cout << "File " << filename << " could not be opened..." << endl;
        }

        tilesRevealed = 0;

        if (n != 1)
            testMines = temp;
        else
            testMines = 1;

        if (isTestBoard)
            totalTilesWithoutMines = (rows * columns) - testMines;

        createBoardBasedOffInts();
        file.close();
    }

    void resetBoard(sf::Event event){
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (smiley.getGlobalBounds().contains(mousePos)){
                createRandomIntBoard();
                createBoardBasedOffInts();
                startTimer();
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (faceLose.getGlobalBounds().contains(mousePos)){
                createRandomIntBoard();
                createBoardBasedOffInts();
                startTimer();
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (faceWin.getGlobalBounds().contains(mousePos)){
                createRandomIntBoard();
                createBoardBasedOffInts();
                startTimer();
            }
        }
    }

    void countSurroundingMines(){
        int count = 0;
        for (int i = 0; i < rows; i++) {
            vector<Tile*> nbs;
            for (int k = 0; k < columns; k++) {
                nbs = tiles[i][k].getNeighbors();
                count = 0;

                for (int j = 0; j < 8; j++)
                    if (nbs.at(j) != nullptr && nbs.at(j)->getHasMine())
                        count++;

                tiles[i][k].setMines(count);
            }
        }
    }

    void updateMineCounter(sf::RenderWindow& window) {
        //cout << mines << endl;
        //cout << totalFlagged << endl;
        if (gameWon) {
            mineCount = 0;
        } else if (isTestBoard) {
            mineCount = testMines - totalFlagged;
        } else {
            mineCount = mines - totalFlagged;
        }

        int ones = 0;
        int tens = 0;
        int hundreds = 0;

        firstDigitSprite.setTexture(Texture::getTexture("digits"));
        secondDigitSprite.setTexture(Texture::getTexture("digits"));
        thirdDigitSprite.setTexture(Texture::getTexture("digits"));
        negative.setTexture(Texture::getTexture("digits"));

        if (mineCount < 0) {
            mineCount *= -1;
            isNegativeMines = true;
        } else {
            isNegativeMines = false;
        }

        if (mineCount >= 100) {
            ones = mineCount % 10;
            mineCount = mineCount / 10;

            tens = mineCount % 10;
            mineCount = mineCount / 10;

            hundreds = mineCount % 10;
        } else if (mineCount >= 10 && mineCount <= 99) {
            ones = mineCount % 10;
            mineCount = mineCount / 10;

            tens = mineCount;
        } else if (mineCount < 10 && mineCount >= 0) {
            ones = mineCount;
        }

        firstDigitSprite.setTextureRect(sf::IntRect(21 * hundreds, 0, 21, 32));
        secondDigitSprite.setTextureRect(sf::IntRect(21 * tens, 0, 21, 32));
        thirdDigitSprite.setTextureRect(sf::IntRect(21 * ones, 0, 21, 32));
        negative.setTextureRect(sf::IntRect(210, 0, 21, 32));

        window.draw(firstDigitSprite);
        window.draw(secondDigitSprite);
        window.draw(thirdDigitSprite);

        if (isNegativeMines) {
            negative.setPosition(12, 32 * ((rows)+0.5f) + 16);
            window.draw(negative);
        } else {
            negative.setPosition(-100, -100);
            window.draw(negative);

        }
    }

    void recursiveClick(Tile* tile){
        //cout << "Tiles without mines: " << totalTilesWithoutMines << endl;
        //cout << "Tiles revealed: " << tilesRevealed << endl;

        if (isTestBoard && totalTilesWithoutMines == tilesRevealed) {
            gameWon = true;
            cout << "Game Won!" << endl;
            return;
        }

        if (totalTilesWithoutMines == tilesRevealed) {
            gameWon = true;
        }

        if (tile == nullptr) {
            return;
        }

        if (!tile->getIsHidden() || tile->getIsFlagged()) {
            return;
        }

        if (tile->getNearbyMines() == 0) {
            tile->revealTile();
            tilesRevealed++;

            vector<Tile*> nbs = tile->getNeighbors();

            for (int i = 0; i < nbs.size(); i++)
                recursiveClick(nbs[i]);

        }
        else {
            tile->revealTile();
            tilesRevealed++;
        }

    }
    void endGame(sf::RenderWindow & window){
        if (gameLost) {
            faceLose.setPosition(((columns / 2.0) * 32) - 32, 32 * (rows + 0.5f));
            window.draw(faceLose);
            return;
        }
        if (gameWon) {
            faceWin.setPosition(((columns / 2.0) * 32) - 32, 32 * (rows + 0.5f));
            window.draw(faceWin);
            endTime = getElapsedTime();
            return;
        }
    }

    void debugMine(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (debug.getGlobalBounds().contains(mousePos)) {
                if(isDebug == false){
                    isDebug = true;
                }
                else{
                    isDebug = false;
                }
                if (isDebug) {
                    for (int i = 0; i < rows; i++) {
                        for (int k = 0; k < columns; k++) {
                            if(tiles[i][k].getHasMine()) {
                                //tiles[i][k].revealTile();
                            }
                        }
                    }
                }
                if (!isDebug) {
                    bool debugOff = true;
                    for (int i = 0; i < rows; i++) {
                        for (int k = 0; k < columns; k++) {
                            if(tiles[i][k].getHasMine()) {
                                tiles[i][k].unRevealTile();
                            }
                        }
                    }
                }
            }
        }
    }


    void leaderboardWindow(sf::Event event, sf::RenderWindow& window, string inputText) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (leaderboard.getGlobalBounds().contains(mousePos)) {
                pauseTime = std::chrono::steady_clock::now();
                for (int i = 0; i < rows; i++) {
                    for (int k = 0; k < columns; k++) {
                        if (tiles[i][k].getIsHidden()) {
                            sf::Sprite tile;
                            tile.setTexture(Texture::getTexture("tile_revealed"));
                            tile.setPosition(k * 32, i * 32);
                            window.draw(tile);
                        }
                    }
                }
                window.display();
                int leaderboard_width = 16 * columns;
                int leaderboard_height = (rows * 16) + 50;
                sf::RenderWindow leaderboardWindow(sf::VideoMode(leaderboard_width, leaderboard_height), "Leaderboard");
                leaderboardWindow.setPosition(sf::Vector2i(window.getPosition().x + (window.getSize().x - leaderboard_width) / 2, window.getPosition().y + (window.getSize().y - leaderboard_height) / 2));
                sf::RectangleShape shape(sf::Vector2f(leaderboard_width, leaderboard_height));
                shape.setFillColor(sf::Color::Blue);

                std::map<int, std::string> leaderboardData;
                sf::Text text;
                sf::Font font;
                font.loadFromFile("font.ttf");
                text.setFont(font);
                text.setString("LEADERBOARD");
                text.setCharacterSize(20);
                text.setColor(sf::Color::White);
                text.setStyle(sf::Text::Bold | sf::Text::Underlined);
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                text.setPosition(sf::Vector2f(leaderboard_width / 2.0f, leaderboard_height / 2.0f - 120));

                sf::Text text2;
                text2.setFont(font);
                text2.setString("");
                text2.setCharacterSize(18);
                text2.setColor(sf::Color::White);
                text2.setStyle(sf::Text::Bold);
                sf::FloatRect textRect2 = text2.getLocalBounds();
                text2.setOrigin(textRect2.left + textRect2.width / 2.0f, textRect2.top + textRect2.height / 2.0f);
                text2.setPosition(sf::Vector2f(leaderboard_width / 2.0f - 120, leaderboard_height / 2.0f - 75));
                string output = "";
                map<int, string> scores;
                ifstream file("leaderboard.txt");
                string line = "";
                while (getline(file, line)) {
                    string score;
                    string name;
                    int num;
                    stringstream string(line);
                    getline(string, score, ',');
                    getline(string, name);
                    num = stoi(score.substr(0, score.find(':'))) * 60 + stoi(score.substr(score.find(':') + 1));
                    scores.emplace(num, name);
                    line = "";
                }
                std::string timeE = std::to_string(endTime);
                std::string time;
                if (endTime < 60) {
                    time = std::to_string((int)endTime);
                }
                else {
                    int minutes = (int)endTime / 60;
                    int seconds = (int)endTime % 60;
                    time = std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
                }
                inputText = inputText + "*";
                if(gameWon){
                    scores.emplace(std::make_pair((int)endTime, inputText));
                }
                map<int, string>::iterator itr;
                int counter = 1;
                for (itr = scores.begin(); itr != scores.end(); itr++) {
                    if ((int) itr->first / 60 < 10 && (int) itr->first % 60 < 10) {
                        output += to_string(counter) + ".\t" + "0" + to_string((int) itr->first / 60) + ":" + "0" +
                                  to_string((int) itr->first % 60) + "\t" + itr->second + "\n\n";
                    } else if ((int) itr->first / 60 < 10) {
                        output += to_string(counter) + ".\t" + "0" + to_string((int) itr->first / 60) + ":" +
                                  to_string((int) itr->first % 60) + "\t" + itr->second + "\n\n";
                    } else if ((int) itr->first % 60 < 10) {
                        output += to_string(counter) + ".\t" + to_string((int) itr->first / 60) + ":" + "0" +
                                  to_string((int) itr->first % 60) + "\t" + itr->second + "\n\n";
                    } else {
                        output += to_string(counter) + ".\t" + to_string((int) itr->first / 60) + ":" +
                                  to_string((int) itr->first % 60) + "\t" + itr->second + "\n\n";
                    }
                    counter++;
                    if (counter == 6) {
                        break;
                    }
                }
                text2.setString(output);
                while (leaderboardWindow.isOpen()) {
                    while (leaderboardWindow.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                            leaderboardWindow.close();
                        }
                    }
                    leaderboardWindow.clear();
                    leaderboardWindow.draw(shape);
                    leaderboardWindow.draw(text);
                    leaderboardWindow.draw(text2);
                    leaderboardWindow.display();
                }
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - pauseTime);
                startTime += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(elapsed);
            }
        }
    }

};