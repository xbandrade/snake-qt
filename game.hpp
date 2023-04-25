#ifndef GAME_HPP
#define GAME_HPP

#include "snake.hpp"
#include <random>
#include <vector>
#include <algorithm>

class Game
{
public:
    Game(std::mt19937 gen);
    void updateFood(Snake *snake);
    int getFoodX();
    int getFoodY();
private:
    int foodX, foodY;
    std::mt19937 m_gen;
};

#endif
