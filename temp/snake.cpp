#include <bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define GRID_SIZE 40
#define CELL_SIZE 20
#define INITIAL_SNAKE_LENGTH 4
#define TIMEOUT 200

int window;
int delay = TIMEOUT;
list<pair<int, int>> velocityBuffer;
int velX, velY, fruitX, fruitY, score = 0, lives = 3;
bool eaten = true, dead = false;
vector<pair <int, int>> snake;
void * font = GLUT_BITMAP_TIMES_ROMAN_24;

void myInit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(0, 800, 0, 850);

    // Initial snake creation
    for(int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
        snake.push_back(make_pair(GRID_SIZE / 2 - i, GRID_SIZE / 2));

    // Initial snake velocities
    velX = 1;
    velY = 0;
}

void menu(int value)
{
    switch (value) {
        case 0:
        {
            glutDestroyWindow(window);
            exit(0);
        }
        case 1:
        {
            snake.clear();
            for(int i = 0; i < INITIAL_SNAKE_LENGTH; i++)
                snake.push_back(make_pair(GRID_SIZE / 2 - i, GRID_SIZE / 2));

            velX = 1;
            velY = 0;

            dead = false;
            score = 0;
            lives = 3;
        }
    }
}

void createMenu()
{
    int menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Restart", 1);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void drawMaze()
{
    glColor3f(1, 1, 1);
    glRecti(5, 15, GRID_SIZE * CELL_SIZE - 5, 5);
    glRecti(5, GRID_SIZE * CELL_SIZE - 5, GRID_SIZE * CELL_SIZE - 5, GRID_SIZE * CELL_SIZE - 15);
    glRecti(5, GRID_SIZE * CELL_SIZE - 5, 15, 5);
    glRecti(GRID_SIZE * CELL_SIZE - 15, GRID_SIZE * CELL_SIZE - 5, GRID_SIZE * CELL_SIZE - 5, 5);
}

void drawRect(int x, int y, int r, int g, int b)
{
    glColor3f(r, g, b);
    glRecti(x * CELL_SIZE + 1, y * CELL_SIZE + 1, (x + 1) * CELL_SIZE - 2, (y + 1) * CELL_SIZE - 2);
}

void drawFruit()
{
    if(eaten)
    {
        fruitX = rand() % 38 + 1;
        fruitY = rand() % 38 + 1;
    }

    drawRect(fruitX, fruitY, 1, 0, 0);
    eaten = false;
}

void drawSnake()
{
    for(int i = 0; i < snake.size(); i++)
        drawRect(snake[i].first, snake[i].second, 0, 1, 0);
}

void renderString(float x, float y, float r, float g, float b, string s)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);

    for(int i = 0; i < s.length(); i++)
        glutBitmapCharacter(font, s[i]);
}

void death()
{
    glClear(GL_COLOR_BUFFER_BIT);
    renderString((GRID_SIZE / 2 - 4) * CELL_SIZE, (GRID_SIZE + 2) / 2 * CELL_SIZE, 1, 1, 1, "GAME OVER!");
    renderString((GRID_SIZE / 2 - 3) * CELL_SIZE, (GRID_SIZE - 2) / 2 * CELL_SIZE, 1, 1, 1, "SCORE: " + to_string(score));
    velX = velY = 0;
}

void updateSnake()
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

void updateVelocities(int key, int x, int y)
{
    if(dead)
        return;
    int velX, velY;
    switch (key) {
        case GLUT_KEY_UP:
            velX = 0;
            velY = 1;
            break;
        case GLUT_KEY_DOWN:
            velX = 0;
            velY = -1;
            break;
        case GLUT_KEY_RIGHT:
            velX = 1;
            velY = 0;
            break;
        case GLUT_KEY_LEFT:
            velX = -1;
            velY = 0;
            break;
    }
    velocityBuffer.push_back(make_pair(velX, velY));
}

void handleMovement(int value)
{
    glutTimerFunc(delay, handleMovement, 0);
    updateSnake();
    if(!velocityBuffer.empty())
    {
        velX = velocityBuffer.front().first;
        velY = velocityBuffer.front().second;
        velocityBuffer.pop_front();
    }
    glutPostRedisplay();
}

void myDisplay()
{
    string scores_string = "SCORE: ", lives_string = "LIVES: ";
    delay = TIMEOUT - score / 5;
    glClear(GL_COLOR_BUFFER_BIT);
    drawMaze();
    drawSnake();
    drawFruit();
    scores_string += to_string(score);
    renderString(15, GRID_SIZE * CELL_SIZE + 10, 1, 1, 1, scores_string);
    lives_string += to_string(lives);
    renderString((GRID_SIZE - 5) * CELL_SIZE, GRID_SIZE * CELL_SIZE + 10, 1, 1, 1, lives_string);
    if(dead)
        death();
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 850);
    window = glutCreateWindow("Snake");
    myInit();
    createMenu();
    glutDisplayFunc(myDisplay);
    glutTimerFunc(delay, handleMovement, 0);
    glutSpecialFunc(updateVelocities);
    glutMainLoop();
    return 0;
}
