#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

extern std::vector<sf::RectangleShape> grid;
extern std::vector<int> gridData;

void startGame();
void drawGame(sf::RenderWindow& window);

int input(sf::Vector2i dir);
bool interact(bool press);
void update(sf::Vector2i dir, bool press);

void swap();
void staleMate();
void check(int player);
int getWinner();
bool isWon();