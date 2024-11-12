#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

typedef struct
{
    int x;
    int y;
} Entity;

int incX = 1;
int incY = 1;

static Entity hero = {34, 12};
static Entity enemy = {11, 3};

void printHero(int nextX, int nextY)
{
    if (nextX > (MAXX - strlen("H") - 1) || nextX < MINX + 1)
    {
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(hero.x, hero.y);
        printf("H");
    }
    else if (nextY > (MAXY - strlen("H") - 1) || nextY < MINY + 1)
    {
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(hero.x, hero.y);
        printf("H");
    }
    else
    {
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(hero.x, hero.y);
        printf(" ");
        hero.x = nextX;
        hero.y = nextY;
        screenGotoxy(hero.x, hero.y);
        printf("H");
    }
}

void moveHero(int direction)
{
    if (direction == 119)
    {
        printHero(hero.x, hero.y - incY);
    }
    else if (direction == 115)
    {
        printHero(hero.x, hero.y + incY);
    }
    else if (direction == 97)
    {
        printHero(hero.x - incX, hero.y);
    }
    else if (direction == 100)
    {
        printHero(hero.x + incX, hero.y);
    }
}

void printEnemy(int nextX, int nextY)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(enemy.x, enemy.y);
    printf(" ");
    enemy.x = nextX;
    enemy.y = nextY;
    screenGotoxy(enemy.x, enemy.y);
    printf("*");
}

void moveEnemy()
{
    if (hero.x > enemy.x)
    {
        printEnemy(enemy.x + incX, enemy.y);
    }
    else if (hero.x < enemy.x)
    {
        printEnemy(enemy.x - incX, enemy.y);
    }
    if (hero.y > enemy.y)
    {
        printEnemy(enemy.x, enemy.y + 1);
    }
    else if (hero.y < enemy.y)
    {
        printEnemy(enemy.x, enemy.y - 1);
    }
}

void printScore(int score)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(MINX + 2, MINY);
    printf(" Pontuação: %d ", score);
}

int main()
{
    static int ch = 0;
    static int elapsedTimeCounter = 0;
    static int score = 0;
    static int enemySpeed = 10;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    printHero(hero.x, hero.y);
    printEnemy(enemy.x, enemy.y);
    printScore(score);
    screenUpdate();

    while (ch != 10)
    {
        if (keyhit())
        {
            ch = readch();
            moveHero(ch);
            screenUpdate();
        }

        if (timerTimeOver() == 1)
        {
            elapsedTimeCounter += 1;
            if (elapsedTimeCounter % enemySpeed == 0)
            {
                moveEnemy();
                screenUpdate();
            }

            if (elapsedTimeCounter % 20 == 0) {
                score += 1;
                printScore(score);
                screenUpdate();
            }
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}