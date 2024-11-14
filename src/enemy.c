#include <stdio.h>

#include "enemy.h"
#include "screen.h"
#include "hero.h"

void printEnemy(Enemy *enemy, int nextX, int nextY)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(enemy->x, enemy->y);
    printf(" ");
    enemy->x = nextX;
    enemy->y = nextY;
    screenGotoxy(enemy->x, enemy->y);
    printf("*");
}

void moveEnemy(Enemy *enemy, Entity *hero)
{
    if (hero->x > enemy->x)
    {
        printEnemy(enemy, enemy->x + 1, enemy->y);
    }
    else if (hero->x < enemy->x)
    {
        printEnemy(enemy, enemy->x - 1, enemy->y);
    }
    if (hero->y > enemy->y)
    {
        printEnemy(enemy, enemy->x, enemy->y + 1);
    }
    else if (hero->y < enemy->y)
    {
        printEnemy(enemy, enemy->x, enemy->y - 1);
    }
}