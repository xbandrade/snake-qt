#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "snake.hpp"
#include "game.hpp"
#include <QWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QPushButton>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QRect>
#include <QMouseEvent>
#include <deque>
#include <random>
#include <iostream>


class Window : public QWidget{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = 0);
signals:
    void snakeDied();
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
private slots:
    void updateWindow();
    void restartWidgets();
private:
    void updateSnakeHead();
    void updateFoodPosition(bool constructorCall = false);
    std::mt19937 m_gen;
    Snake *snake;
    Game *game;
    QWidget *gridContainerWrapper, *gridContainer;
    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;
    QTimer *updateTimer, *gameOverTimer;
    std::deque<QWidget *> snakeBodyWidgets;
    QVBoxLayout* gridCellLayout;
    QHBoxLayout* gridCellHBoxLayout;
    QWidget *foodWidget;
    int squareSize, cellSize, boardX, boardY;
};
#endif
