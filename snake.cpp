#include "snake.h"

using namespace std;

Snake::Snake()
{
    // Initial snake creation
    for(int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
        snake.push_back(make_pair(GRID_SIZE / 2 - i, GRID_SIZE / 2));

    // Initial snake velocities
    velX = 1;
    velY = 0;

    score = 0;
    lives = 3;
    eaten = true;
    dead = false;
}

int Snake::getLives()
{
    return lives;
}

int Snake::getScore()
{
    return score;
}

bool Snake::getDead()
{
    return dead;
}

void Snake::drawMaze()
{
    glColor3f(1, 1, 1);
    glRecti(5, 15, GRID_SIZE * CELL_SIZE - 5, 5);
    glRecti(5, GRID_SIZE * CELL_SIZE - 5, GRID_SIZE * CELL_SIZE - 5, GRID_SIZE * CELL_SIZE - 15);
    glRecti(5, GRID_SIZE * CELL_SIZE - 5, 15, 5);
    glRecti(GRID_SIZE * CELL_SIZE - 15, GRID_SIZE * CELL_SIZE - 5, GRID_SIZE * CELL_SIZE - 5, 5);
}

void Snake::drawRect(int x, int y, int r, int g, int b)
{
    glColor3f(r, g, b);
    glRecti(x * CELL_SIZE + 1, y * CELL_SIZE + 1, (x + 1) * CELL_SIZE - 2, (y + 1) * CELL_SIZE - 2);
}

void Snake::drawFruit()
{
    if(eaten)
    {
        fruitX = rand() % 38 + 1;
        fruitY = rand() % 38 + 1;
    }

    drawRect(fruitX, fruitY, 1, 0, 0);
    eaten = false;
}

void Snake::drawSnake()
{
    for(int i = 0; i < snake.size(); i++)
        drawRect(snake[i].first, snake[i].second, 0, 1, 0);
}

void Snake::death()
{
    velX = velY = 0;
}

void Snake::updateSnake()
{
    if(snake[0].first == GRID_SIZE - 1 || snake[0].first == 0 || snake[0].second == GRID_SIZE - 1 || snake[0].second == 0)
    {
        if(lives--)
        {
            snake.clear();
            for(int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
                snake.push_back(make_pair(GRID_SIZE / 2 - i, GRID_SIZE / 2));

            velX = 1;
            velY = 0;

            return;
        }
        else
            dead = true;
    }

    int x = snake[0].first;
    int y = snake[0].second;

    int last_x = snake[snake.size() - 1].first;
    int last_y = snake[snake.size() - 1].second;

    drawRect(last_x, last_y, 0, 0, 0);

    for(int i = snake.size() - 1; i >= 1; i--)
        snake[i] = snake[i - 1];

    x += velX;
    y += velY;
    snake[0] = make_pair(x, y);

    if(fruitX == x && fruitY == y)
    {
        snake.push_back(make_pair(0, 0));
        eaten = true;

        for(int i = snake.size() - 1; i >= 1; i--)
            snake[i] = snake[i - 1];

        snake[0] = make_pair(x, y);
        score++;
    }

    for(int i = 2; i < snake.size(); i++)
        if(snake[0].first == snake[i].first && snake[0].second == snake[i].second)
        {
            if(lives--)
            {
                snake.clear();
                for(int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
                    snake.push_back(make_pair(GRID_SIZE / 2 - i, GRID_SIZE / 2));

                velX = 1;
                velY = 0;

                return;
            }
            else
                dead = true;
        }
}

void Snake::updateVelocities(int x, int y)
{
    if(velX == 1 && x == -1 || velX == -1 && x == 1 || velY == 1 && y == -1 || velY == -1 && y == 1)
        return;
    velX = x;
    velY = y;
}

void Snake::reset()
{
    snake.clear();
    snake.push_back(make_pair(GRID_SIZE / 2, GRID_SIZE / 2));
    snake.push_back(make_pair(GRID_SIZE / 2 - 1, GRID_SIZE / 2));
    snake.push_back(make_pair(GRID_SIZE / 2 - 2, GRID_SIZE / 2));
    snake.push_back(make_pair(GRID_SIZE / 2 - 3, GRID_SIZE / 2));

    velX = 1;
    velY = 0;

    dead = false;
    score = 0;
    lives = 3;
}
