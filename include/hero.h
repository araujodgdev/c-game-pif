#ifndef __HERO_H__
#define __HERO_H__

typedef struct
{
    int x;
    int y;
} Entity;

void printHero(Entity *hero, int nextX, int nextY);

void moveHero(Entity *hero, int direction);

#endif /* __HERO_H__ */