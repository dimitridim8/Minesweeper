#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <chrono>
#include <vector>
#include <fstream>
using namespace std;

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main()
{
    sf::RenderWindow windowW(sf::VideoMode(800, 600), "Minesweeper");
    sf::RectangleShape shape(sf::Vector2f(800, 600));

      shape.setFillColor(sf::Color::Blue);
      sf::Text text;
      sf::Font font;
      font.loadFromFile("font.ttf");
      text.setFont(font);
      text.setString("Welcome To Minesweeper!");
      text.setCharacterSize(24);
      text.setFillColor(sf::Color::White);
      text.setStyle(sf::Text::Bold | sf::Text::Underlined);
      sf::Text text2;
      text2.setFont(font);
      text2.setString("Enter your name:");
      text2.setCharacterSize(20);
      text2.setFillColor(sf::Color::White);
      text2.setStyle(sf::Text::Bold);

      sf::Text userInput("|", font, 18);
      userInput.setFillColor(sf::Color::Yellow);
      userInput.setStyle(sf::Text::Bold);
      //userInput.setPosition(centerX, centerY + textBounds.height + 50.f);
      std::string inputText;

      bool enterPressed = false;

      //userInput.setPosition(centerX + text2Bounds.width / 2.f, centerY + textBounds.height + 50.f);
      sf::Vector2f cursorPosition = userInput.getPosition();
      sf::Vertex cursorLine[] = {
            sf::Vertex(sf::Vector2f(userInput.getPosition().x - userInput.getLocalBounds().width / 2.f, userInput.getPosition().y), sf::Color::Yellow),
            sf::Vertex(sf::Vector2f(userInput.getPosition().x + userInput.getLocalBounds().width / 2.f, userInput.getPosition().y), sf::Color::Yellow)
      };
      windowW.draw(cursorLine, 2, sf::Lines);

    while (windowW.isOpen()) {
        sf::Event event;
        while (windowW.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowW.close();
                return 0;
            }

            if (event.type == sf::Event::TextEntered) {
                if (inputText.size() < 10) {
                    if (event.text.unicode >= 32 && event.text.unicode < 128) {
                        if (isalpha(event.text.unicode)) {
                            inputText += static_cast<char>(event.text.unicode);
                            userInput.setString(inputText + "|");
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!inputText.empty()) {
                        inputText.pop_back();
                        userInput.setString(inputText + "|");
                    }
                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Return) {
                        enterPressed = true;
                        std::string formattedName = inputText;
                        if (!formattedName.empty()) {
                            formattedName[0] = toupper(formattedName[0]);
                            for (size_t i = 1; i < formattedName.size(); ++i) {
                                formattedName[i] = tolower(formattedName[i]);
                            }
                        }
                        inputText = formattedName;
                        if (!inputText.empty()) {
                            windowW.close();
                        }
                    }
                }
            }
        }

        sf::FloatRect textRect = text.getLocalBounds();
        setText(text, windowW.getSize().x / 2.f, windowW.getSize().y / 2.f);
        setText(text2, windowW.getSize().x / 2.f, (windowW.getSize().y / 2.f) + textRect.height + 30.f);
        setText(userInput, windowW.getSize().x / 2.f, (windowW.getSize().y / 2.f) + textRect.height + 50.f);

        windowW.clear();
        windowW.clear();
        windowW.draw(shape);
        windowW.draw(text);
        windowW.draw(text2);
        windowW.draw(userInput);
        windowW.display();

    }

    Board board;
    Tile tile;
    board.setDimensions();
    sf::RenderWindow window(sf::VideoMode(board.getWidth(), board.getHeight()), "Minesweeper");

    board.createRandomIntBoard();
    board.createBoardBasedOffInts();
    board.startTimer();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            board.resetBoard(event);
        }

        board.drawBoard(window);
        board.drawBoardSprites(window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            board.checkTileClicked(window);
            continue;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            board.updateMineCounter(window);
            board.flagATile(window);
            continue;
        }

        board.handlePlayPause(event, window);
        board.debugMine(event);
        board.updateTimer();
        window.display();
        board.leaderboardWindow(event, window, inputText);

    }
    return 0;
}
