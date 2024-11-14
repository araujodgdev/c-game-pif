#include <stdio.h>
#include <unistd.h>

#include "enemy.h"
#include "screen.h"
#include "hero.h"
#include "keyboard.h"
#include "timer.h"
#include "score.h"

void printEnemy(Enemy *enemy, int nextX, int nextY)
{
    screenSetColor(RED, DARKGRAY);
    screenGotoxy(enemy->x, enemy->y);
    printf(" ");
    enemy->x = nextX;
    enemy->y = nextY;
    screenGotoxy(enemy->x, enemy->y);
    printf("👾");
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

void spawnEnemy(EnemyManager *manager)
{
    if (manager->count >= MAX_ENEMIES)
        return;

    int edge;
    Enemy newEnemy;
    int attempts = 0;
    const int maxAttempts = 10;

    while (attempts < maxAttempts)
    {
        edge = rand() % 4;
        switch (edge)
        {
        case 0: // Topo
            newEnemy.x = rand() % (MAXX - MINX - 2) + MINX + 1;
            newEnemy.y = MINY + 1;
            break;
        case 1: // Baixo
            newEnemy.x = rand() % (MAXX - MINX - 2) + MINX + 1;
            newEnemy.y = MAXY - 1;
            break;
        case 2: // Esquerda
            newEnemy.x = MINX + 1;
            newEnemy.y = rand() % (MAXY - MINY - 2) + MINY + 1;
            break;
        case 3: // Direita
            newEnemy.x = MAXX - 4;
            newEnemy.y = rand() % (MAXY - MINY - 2) + MINY + 1;
            break;
        }

        if (!isPositionOccupied(manager, newEnemy.x, newEnemy.y))
        {
            newEnemy.speed = ENEMY_SPEED;

            manager->enemies[manager->count++] = newEnemy;

            printEnemy(&newEnemy, newEnemy.x, newEnemy.y);
            return;
        }

        attempts++;
    }
}

int updateEnemies(EnemyManager *manager, Entity *hero, int score)
{
    for (int i = 0; i < manager->count;)
    {
        int desiredX, desiredY;
        getNextPosition(&manager->enemies[i], hero, &desiredX, &desiredY);

        // Limita os valores de x e y dentro dos limites da tela
        if (desiredX < MINX + 1)
            desiredX = MINX + 1;
        if (desiredX >= MAXX - 1)
            desiredX = MAXX - 2; // -2 para evitar a borda
        if (desiredY < MINY + 1)
            desiredY = MINY + 1;
        if (desiredY >= MAXY - 1)
            desiredY = MAXY - 2; // -2 para evitar a borda

        // Verifica se a próxima posição está ocupada por outro inimigo
        if (!isPositionOccupied(manager, desiredX, desiredY))
        {
            // Limpa a posição antiga do inimigo
            screenGotoxy(manager->enemies[i].x, manager->enemies[i].y);
            printf(" ");

            // Atualiza a posição do inimigo
            manager->enemies[i].x = desiredX;
            manager->enemies[i].y = desiredY;

            // Renderiza a nova posição do inimigo
            screenSetColor(CYAN, DARKGRAY);
            screenGotoxy(manager->enemies[i].x, manager->enemies[i].y);
            printf("👾");
        }
        else
        {
            // Se a posição está ocupada, o inimigo não se move
            // Alternativamente, você pode implementar uma lógica para contornar ou mudar de direção
        }

        // Verifica se o inimigo alcançou o herói
        if (manager->enemies[i].x == hero->x && manager->enemies[i].y == hero->y)
        {
            // Diminui uma vida
            hero->lives -= 1;

            // Limpa a posição do inimigo
            screenGotoxy(manager->enemies[i].x, manager->enemies[i].y);
            printf(" ");

            // Remove o inimigo da lista
            manager->enemies[i] = manager->enemies[manager->count - 1];
            manager->count--;

            // Verifica se o herói ainda tem vidas
            if (hero->lives <= 0)
            {
                printf("\nO inimigo alcançou o herói! Game Over.\n");
                saveScore(score); // Salva a pontuação antes de sair
                keyboardDestroy();
                screenDestroy();
                timerDestroy();
                screenClear();
                fflush(stdout);
                exit(0);
            }
            else
            {
                sleep(1); // Requer #include <unistd.h>
            }
        }

        // Verifica se o inimigo saiu da tela (não necessário se movem sempre em direção ao herói)
        if (manager->enemies[i].x < MINX + 1 || manager->enemies[i].x >= MAXX - 1 ||
            manager->enemies[i].y < MINY + 1 || manager->enemies[i].y >= MAXY - 1)
        {
            // Limpa a posição do inimigo removido
            screenGotoxy(manager->enemies[i].x, manager->enemies[i].y);
            printf(" ");

            // Remove o inimigo da lista
            manager->enemies[i] = manager->enemies[manager->count - 1];
            manager->count--;
            continue; // Não incrementa i, pois um novo inimigo está na posição i
        }

        i++; // Incrementa apenas se o inimigo não foi removido
    }

    // Após atualizar todos os inimigos, verifica se houve colisão
    return checkCollision(hero, manager);
}

int isPositionOccupied(const EnemyManager *manager, int x, int y)
{
    for (int i = 0; i < manager->count; i++)
    {
        if (manager->enemies[i].x == x && manager->enemies[i].y == y)
            return 1;
    }
    return 0;
}

void getNextPosition(const Enemy *enemy, const Entity *hero, int *newX, int *newY)
{
    *newX = enemy->x;
    *newY = enemy->y;

    if (hero->x > enemy->x)
    {
        *newX += 1;
    }
    else if (hero->x < enemy->x)
    {
        *newX -= 1;
    }

    if (hero->y > enemy->y)
    {
        *newY += 1;
    }
    else if (hero->y < enemy->y)
    {
        *newY -= 1;
    }
}

int checkCollision(const Entity *hero, const EnemyManager *manager)
{
    for (int i = 0; i < manager->count; i++)
    {
        if (manager->enemies[i].x == hero->x && manager->enemies[i].y == hero->y)
            return 1;
    }
    return 0;
}
