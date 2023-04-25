#include "game.hpp"

Game::Game(std::mt19937 gen)
{
    m_gen = gen;
    std::uniform_int_distribution<> dis(0, 3);
}


void Game::updateFood(Snake *snake){
    std::deque<std::pair<int, int>> body = snake->getBody();
    int limX = snake->getBoardLimX(), limY = snake->getBoardLimY();
    if (body.size() == static_cast<std::size_t>(limX) * limY){
        return;
    }
    std::uniform_int_distribution<> disX(0, limX - 1);
    std::uniform_int_distribution<> disY(0, limY - 1);
    int randX = disX(m_gen), randY = disY(m_gen);
    auto it = std::find(body.begin(), body.end(), std::make_pair(randX, randY));
    while (it != body.end()){
        randX = disX(m_gen);
        randY = disY(m_gen);
        it = std::find(body.begin(), body.end(), std::make_pair(randX, randY));
    }
    foodX = randX;
    foodY = randY;
}


int Game::getFoodX(){
    return foodX;
}


int Game::getFoodY(){
    return foodY;
}


