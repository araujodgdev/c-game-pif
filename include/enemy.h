#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <stdlib.h>
#include <hero.h>

#define MAX_ENEMIES 100
#define SPAWN_INTERVAL 30 
#define ENEMY_SPEED 10 

typedef struct
{
    int x;
    int y;
    int speed;
} Enemy;

typedef struct {
    Enemy* array;
    size_t used;
    size_t capacity;
} EnemyList;

typedef struct {
    Enemy enemies[MAX_ENEMIES];
    int count;
} EnemyManager;

void printEnemy(Enemy *enemy, int nextX, int nextY);

void moveEnemy(Enemy *enemy, Entity *hero);

void spawnEnemy(EnemyManager *manager);

int updateEnemies(EnemyManager *manager, Entity *hero, int score);

int isPositionOccupied(const EnemyManager *manager, int x, int y);

void getNextPosition(const Enemy *enemy, const Entity *hero, int *newX, int *newY);

int checkCollision(const Entity *hero, const EnemyManager *manager);


#endif /* __ENEMY_H__ */