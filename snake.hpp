#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <vector>
#include <algorithm>
#include <deque>
#include <map>

class Snake{
public:
    Snake();
    int move(int foodX, int foodY);
    bool setDir(int direction);
    bool getDead();
    int getX();
    int getY();
    int getBoardLimX();
    int getBoardLimY();
    const std::deque<std::pair<int, int>> &getBody();
private:
    int x, y, dir;  // dir 0: up, 1: left, 2: down, 3: right
    bool dead;
    std::deque<std::pair<int, int>> body;
    std::vector<std::pair<int, int>> directions;
    std::map<int, int> previous;
    int boardLimX, boardLimY;
    int squareSize;
};

#endif
