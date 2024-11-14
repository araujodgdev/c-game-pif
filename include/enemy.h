#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <stdlib.h>
#include <hero.h>

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

void printEnemy(Enemy *enemy, int nextX, int nextY);


void moveEnemy(Enemy *enemy, Entity *hero);


#endif /* __ENEMY_H__ */