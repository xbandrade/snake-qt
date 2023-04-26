#include "snake.hpp"

Snake::Snake(){
    x = 5;
    y = 5;
    dir = 0;
    dead = false;
    body.push_back({x, y});
    directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    previous = {{0, 2}, {1, 3}, {2, 0}, {3, 1}};
    boardLimX = 10;
    boardLimY = 10;
}


int Snake::move(int foodX, int foodY){
    // -1: snake died, 0: snake moved, 1: snake grew
    std::pair<int, int> direction = directions[dir];
    x += direction.first;
    y += direction.second;
    if (x < 0){
        x = boardLimX - 1;
    }
    else if (x >= boardLimX){
        x = 0;
    }
    if (y < 0){
        y = boardLimY - 1;
    }
    else if (y >= boardLimY){
        y = 0;
    }
    auto it = std::find(body.begin(), body.end(), std::make_pair(x, y));
    if (it != body.end()){
        body.clear();
        body.push_back({5, 5});
        x = 5;
        y = 5;
        return -1;
    }
    if (x == foodX && y == foodY){
        body.push_front({x, y});
        return 1;
    }
    body.pop_back();
    body.push_front({x, y});
    return 0;
}

void Snake::setNewHead(std::pair<int, int> newHead){
    body.clear();
    body.push_back(newHead);
    x = newHead.first;
    y = newHead.second;
}


bool Snake::setDir(int direction){
    if (direction == previous[dir]){
        return false;
    }
    dir = direction;
    return true;
}

bool Snake::getDead(){
    return dead;
}

int Snake::getX(){
    return x;
}

int Snake::getY(){
    return y;
}

int Snake::getBoardLimX(){
    return boardLimX;
}

int Snake::getBoardLimY(){
    return boardLimY;
}

const std::deque<std::pair<int, int>> &Snake::getBody(){
    return body;
}

