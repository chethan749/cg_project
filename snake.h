#include <bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define GRID_SIZE 40
#define CELL_SIZE 20
#define INITIAL_SNAKE_LENGTH 4
#define TIMEOUT 100

class Snake
{
    int velX, velY, fruitX, fruitY, score, lives;
    bool eaten, dead;
    vector<pair <int, int>> snake;

public:
    Snake();
    int getLives();
    int getScore();
    bool getDead();
    void drawMaze();
    void drawRect(int x, int y, int r, int g, int b);
    void drawFruit();
    void drawSnake();
    void death();
    void updateSnake();
    void updateVelocities(int x, int y);
    void reset();
};
