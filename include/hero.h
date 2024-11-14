#ifndef __HERO_H__
#define __HERO_H__

typedef struct
{
    int x;
    int y;
    int lives;
} Entity;

void printHero(Entity *hero, int nextX, int nextY);

void moveHero(Entity *hero, int direction);

void printHeroLives();

#endif /* __HERO_H__ */