#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "enemy.h"
#include "hero.h"

static Entity h = {34, 12};
static Entity* hero = &h; 
static Enemy e = {11, 3, 10};
static Enemy* enemy = &e;


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

    printHero(hero, hero->x, hero->y);
    printEnemy(enemy, enemy->x, enemy->y);
    printScore(score);
    screenUpdate();

    while (ch != 10)
    {
        if (keyhit())
        {
            ch = readch();
            moveHero(hero, ch);
            screenUpdate();
        }

        if (timerTimeOver() == 1)
        {
            elapsedTimeCounter += 1;
            if (elapsedTimeCounter % enemySpeed == 0)
            {
                moveEnemy(enemy, hero);
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