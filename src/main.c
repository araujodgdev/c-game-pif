#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "enemy.h"
#include "hero.h"
#include "score.h"

static Entity h = {34, 12, 3};
static Entity *hero = &h;
static EnemyManager enemyManager = {.count = 0};
static EnemyManager *em = &enemyManager;

void showMenu()
{
    screenClear();
    screenSetColor(LIGHTGREEN, DARKGRAY);
    screenGotoxy(30, 10);
    printf("=== HIGHTALE ===\n\n");

    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(30, 12);
    printf("1. Jogar\n");

    screenGotoxy(30, 13);
    printf("2. Ver Placar\n");

    screenGotoxy(30, 14);
    printf("3. Sair\n");

    fflush(stdout);
}

int getMenuChoice()
{
    int choice = 0;
    char input = 0;

    while (1)
    {
        if (keyhit())
        {
            input = readch();
            if (input >= '1' && input <= '3')
            {
                choice = input - '0';
                break;
            }
        }
    }

    return choice;
}

void playGame()
{
    timerInit(50);

    enemyManager.count = 0;

    screenClear();
    screenDrawBorders();
    printHero(hero, hero->x, hero->y);
    printHeroLives(hero);
    printScore(0);
    screenUpdate();

    int ch = 0;
    int elapsedTimeCounter = 0;
    int score = 0;
    int spawnCounter = 0;

    while (ch != 10)
    {
        if (keyhit())
        {
            ch = readch();
            moveHero(hero, ch);

            screenUpdate();
        }

        if (timerTimeOver())
        {
            elapsedTimeCounter += 1;
            spawnCounter += 1;

            if (spawnCounter >= SPAWN_INTERVAL)
            {
                spawnEnemy(&enemyManager);
                spawnCounter = 0;
            }

            if (elapsedTimeCounter % ENEMY_SPEED == 0)
            {
                updateEnemies(&enemyManager, hero, score);
                printHeroLives(hero);
            }

            screenUpdate();

            if (elapsedTimeCounter % 20 == 0)
            {
                score += 1;
                printScore(score);
            }
        }
    }
    saveScore(score);

    timerDestroy();
}

int main()
{
    srand(time(NULL));

    screenInit(1);
    keyboardInit();

    int choice = 0;
    while (1)
    {
        showMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            playGame();
            break;
        case 2:
            viewScoreboard();
            break;
        case 3:
            keyboardDestroy();
            screenDestroy();
            timerDestroy();
            screenClear();
            fflush(stdout);
            exit(0);
            break;
        default:
            printf("Escolha inválida. Tente novamente.\n");
            break;
        }
    }

    return 0;
}
