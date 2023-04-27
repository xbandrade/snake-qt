#include "window.hpp"


Window::Window(QWidget *parent) : QWidget(parent){
    std::random_device rd;
    m_gen = std::mt19937(rd());
    setMouseTracking(true);
    setAttribute(Qt::WA_MouseTracking);
    snake = new Snake;
    game = new Game(m_gen, snake);
    squareSize = 48;
    cellSize = 50;
    setFixedSize(800, 600);
    setStyleSheet("background-color: #a8bebf;");
    gridContainerWrapper = new QWidget(this);
    boardX = snake->getBoardLimX();
    boardY = snake->getBoardLimY();
    gridContainerWrapper->setFixedSize(boardX * cellSize, boardY * cellSize);
    gridContainerWrapper->setStyleSheet("border: 1px solid black");
    gridContainerWrapper->setAttribute(Qt::WA_MouseTracking);
    gridContainerWrapper->setMouseTracking(true);
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(gridContainerWrapper);
    mainLayout->setAlignment(Qt::AlignCenter);
    gridContainer = new QWidget(gridContainerWrapper);
    gridContainer->setFixedSize(boardX * cellSize, boardY * cellSize);
    gridContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gridContainer->setAttribute(Qt::WA_MouseTracking);
    gridContainer->setMouseTracking(true);
    gridLayout = new QGridLayout(gridContainer);
    gridLayout->setSpacing(0);
    for (int row = 0; row < boardX; ++row) {
        for (int col = 0; col < boardY; ++col) {
            QWidget* gridWidget = new QWidget(gridContainer);
            gridWidget->setStyleSheet("border: 1px dashed gray");
            gridWidget->setMouseTracking(true);
            gridLayout->addWidget(gridWidget, row, col);
        }
    }
    updateSnakeHead();
    updateFoodPosition(true);
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &Window::updateWindow);
    connect(this, &Window::snakeDied, this, &Window::restartWidgets);
    updateTimer->start(150);
}


void Window::updateSnakeHead(){
    QWidget *snakeHeadWidget = new QWidget(gridContainer);
    snakeHeadWidget->setStyleSheet("background-color: #424e4f");
    snakeHeadWidget->setFixedSize(squareSize, squareSize);
    if (snakeBodyWidgets.size() >= 1){
        QWidget *snakeTail = snakeBodyWidgets.front();
        snakeTail->setStyleSheet("background-color: #869c9e");
        snakeTail->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
    snakeBodyWidgets.push_front(snakeHeadWidget);
    int snakeHeadX = snake->getX();
    int snakeHeadY = snake->getY();
    gridLayout->addWidget(snakeHeadWidget, snakeHeadX, snakeHeadY);
}

void Window::updateFoodPosition(bool constructorCall){
    if (!constructorCall){
        QWidget *food = foodWidget;
        gridLayout->removeWidget(food);
        delete food;
    }
    game->updateFood();
    foodWidget = new QWidget(gridContainer);
    foodWidget->setStyleSheet("background-color: #b51b10");
    foodWidget->setFixedSize(squareSize, squareSize);
    foodWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    int foodX = game->getFoodX();
    int foodY = game->getFoodY();
    gridLayout->addWidget(foodWidget, foodX, foodY);
}


void Window::updateWindow(){
    int movement = game->updateSnake();
    if (movement == -1){  // snake died
        QTimer::singleShot(3500, this, [=]() {
            emit snakeDied();
        });
        QEventLoop loop;
        QTimer::singleShot(3500, &loop, &QEventLoop::quit);
        loop.exec();
    }
    else if (movement == 0){  // snake moved
        QWidget *tail = snakeBodyWidgets.back();
        gridLayout->removeWidget(tail);
        snakeBodyWidgets.pop_back();
        delete tail;
        updateSnakeHead();
    }
    else if (movement == 1){  // snake grew
        updateFoodPosition();
        updateSnakeHead();
    }
    gridContainer->update();
}


void Window::restartWidgets(){
    std::cout << "Game Over!" << std::endl;
    while (snakeBodyWidgets.size() > 0){
        QWidget *widget = snakeBodyWidgets.back();
        gridLayout->removeWidget(widget);
        snakeBodyWidgets.pop_back();
        delete widget;
    }
    updateSnakeHead();
    updateFoodPosition();
    snake->setNewHead({5, 5});
}


void Window::mouseMoveEvent(QMouseEvent* event){
    QPoint globalPos = event->globalPosition().toPoint();
    QPoint localPos = gridContainer->mapFromGlobal(globalPos);
    if (gridContainer->rect().contains(localPos)) {
        int row = localPos.y() / cellSize;
        int col = localPos.x() / cellSize;
        if (row >= 0 && row < boardX && col >= 0 && col < boardY) {
            int snakeX = snake->getX(), snakeY = snake->getY();
            int diffX = row - snakeX, diffY = col - snakeY;
            /*std::cout << snake->getY() << " - " << snake->getX() << std::endl;
            std::cout << row << " * " << col << std::endl;
            std::cout << diffX << " ~ " << diffY << std::endl;*/
            if ((diffX < 0) && (diffY < 0)){
                if (diffX <= diffY){
                    snake->setDir(1);
                }
                else {
                    snake->setDir(0);
                }
            }
            else if ((diffX > 0) && (diffY > 0)){
                if (diffX <= diffY){
                    snake->setDir(3);
                }
                else {
                    snake->setDir(2);
                }
            }
            else if ((diffX < 0) && (diffY > 0)){
                if (diffX <= diffY){
                    snake->setDir(3);
                }
                else {
                    snake->setDir(0);
                }
            }
            else if ((diffX > 0) && (diffY < 0)){
                if (diffX <= diffY){
                    snake->setDir(1);
                }
                else {
                    snake->setDir(2);
                }
            }
            else if (diffY > 0){
                snake->setDir(3);
            }
            else if (diffY < 0){
                snake->setDir(1);
            }
            else if (diffX > 0){
                snake->setDir(2);
            }
            else if (diffX < 0){
                snake->setDir(0);
            }
        }
    }
}
