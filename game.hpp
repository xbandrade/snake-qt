#ifndef GAME_HPP
#define GAME_HPP

#include "snake.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <QTimer>
#include <QEventLoop>


class Game {
public:
    Game(std::mt19937 gen, Snake *snk);
    void updateFood();
    int updateSnake();
    int getFoodX();
    int getFoodY();
private:
    Snake *snake;
    int foodX, foodY;
    std::mt19937 m_gen;
};

#endif
