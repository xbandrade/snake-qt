#include "window.hpp"


Window::Window(QWidget *parent) : QWidget(parent){
    std::random_device rd;
    m_gen = std::mt19937(rd());
    snake = new Snake;
    game = new Game(m_gen, snake);
    squareSize = 48;
    setFixedSize(800, 600);
    setStyleSheet("background-color: #a8bebf;");
    gridContainerWrapper = new QWidget(this);
    int boardX = snake->getBoardLimX();
    int boardY = snake->getBoardLimY();
    gridContainerWrapper->setFixedSize(boardX * 50, boardY * 50);
    gridContainerWrapper->setStyleSheet("border: 1px solid black");
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(gridContainerWrapper);
    mainLayout->setAlignment(Qt::AlignCenter);
    gridContainer = new QWidget(gridContainerWrapper);
    gridContainer->setFixedSize(boardX * 50, boardY * 50);
    gridContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gridLayout = new QGridLayout(gridContainer);
    gridLayout->setSpacing(0);
    for (int row = 0; row < boardX; ++row) {
        for (int col = 0; col < boardY; ++col) {
            QWidget* gridWidget = new QWidget(gridContainer);
            gridWidget->setStyleSheet("border: 1px dashed gray");
            gridLayout->addWidget(gridWidget, row, col);
        }
    }
    updateSnakeHead();
    updateFoodPosition(true);
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &Window::updateWindow);
    connect(this, &Window::snakeDied, this, &Window::restartWidgets);
    updateTimer->start(50);
}


void Window::updateSnakeHead(){
    QWidget *snakeHeadWidget = new QWidget(gridContainer);
    snakeHeadWidget->setStyleSheet("background-color: #424e4f");
    snakeHeadWidget->setFixedSize(squareSize, squareSize);
    if (snakeBodyWidgets.size() >= 1){
        QWidget *snakeTail = snakeBodyWidgets.front();
        snakeTail->setStyleSheet("background-color: #869c9e");
    }
    snakeBodyWidgets.push_front(snakeHeadWidget);
    int snakeHeadX = snake->getX();
    int snakeHeadY = snake->getY();
    gridLayout->addWidget(snakeHeadWidget, snakeHeadY, snakeHeadX);
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
    int foodX = game->getFoodX();
    int foodY = game->getFoodY();
    gridLayout->addWidget(foodWidget, foodY, foodX);
}


void Window::updateWindow(){
    std::uniform_int_distribution<> dis(0, 3);
    bool canMove;
    do{
        canMove = snake->setDir(dis(m_gen));
    }while (!canMove);
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
