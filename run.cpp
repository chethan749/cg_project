#include "snake.h"

using namespace std;

int window;
int delay = TIMEOUT;
void * font = GLUT_BITMAP_TIMES_ROMAN_24;

Snake snake_obj;

void myInit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(0, 800, 0, 850);
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
            snake_obj.reset();
            break;
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

void renderString(float x, float y, float r, float g, float b, string s)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);

    for(int i = 0; i < s.length(); i++)
        glutBitmapCharacter(font, s[i]);
}

void updateVelocities(int key, int x, int y)
{
    if(snake_obj.getDead())
        return;
    switch (key) {
        case GLUT_KEY_UP:
            snake_obj.updateVelocities(0, 1);
            break;
        case GLUT_KEY_DOWN:
            snake_obj.updateVelocities(0, -1);
            break;
        case GLUT_KEY_RIGHT:
            snake_obj.updateVelocities(1, 0);
            break;
        case GLUT_KEY_LEFT:
            snake_obj.updateVelocities(-1, 0);
            break;
    }
}

void handleMovement(int value)
{
    glutTimerFunc(delay, handleMovement, 0);
    snake_obj.updateSnake();
    glutPostRedisplay();
}

void end()
{
    glClear(GL_COLOR_BUFFER_BIT);
    renderString((GRID_SIZE / 2 - 4) * CELL_SIZE, (GRID_SIZE + 2) / 2 * CELL_SIZE, 1, 1, 1, "GAME OVER!");
    renderString((GRID_SIZE / 2 - 3) * CELL_SIZE, (GRID_SIZE - 2) / 2 * CELL_SIZE, 1, 1, 1, "SCORE: " + to_string(snake_obj.getScore()));
}

void myDisplay()
{
    string scores_string = "SCORE: ", lives_string = "LIVES: ";
    delay = TIMEOUT - snake_obj.getScore() / 5;
    glClear(GL_COLOR_BUFFER_BIT);
    snake_obj.drawMaze();
    snake_obj.drawSnake();
    snake_obj.drawFruit();
    scores_string += to_string(snake_obj.getScore());
    renderString(15, GRID_SIZE * CELL_SIZE + 10, 1, 1, 1, scores_string);
    lives_string += to_string(snake_obj.getLives());
    renderString((GRID_SIZE - 5) * CELL_SIZE, GRID_SIZE * CELL_SIZE + 10, 1, 1, 1, lives_string);
    if(snake_obj.getDead())
    {
        snake_obj.death();
        end();
    }
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
