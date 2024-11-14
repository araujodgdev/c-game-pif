#include <stdio.h>
#include <string.h>
#include "hero.h"
#include "screen.h"

void printHero(Entity *hero, int nextX, int nextY)
{
    if (nextX > (MAXX - strlen("H") - 1) || nextX < MINX + 1)
    {
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(hero->x, hero->y);
        printf("H");
    }
    else if (nextY > (MAXY - strlen("H") - 1) || nextY < MINY + 1)
    {
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(hero->x, hero->y);
        printf("H");
    }
    else
    {
        screenSetColor(CYAN, DARKGRAY);
        screenGotoxy(hero->x, hero->y);
        printf(" ");
        hero->x = nextX;
        hero->y = nextY;
        screenGotoxy(hero->x, hero->y);
        printf("H");
    }
}

void moveHero(Entity *hero, int direction)
{
    if (direction == 119)
    {
        printHero(hero, hero->x, hero->y - 1);
    }
    else if (direction == 115)
    {
        printHero(hero, hero->x, hero->y + 1);
    }
    else if (direction == 97)
    {
        printHero(hero, hero->x - 1, hero->y);
    }
    else if (direction == 100)
    {
        printHero(hero, hero->x + 1, hero->y);
    }
}
