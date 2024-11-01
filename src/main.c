#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define MAX_MONSTERS 100

typedef struct {
    int x, y;
} Entity;

Entity player;
Entity monsters[MAX_MONSTERS];
int monster_count = 0;
int game_over = 0;

void init_game() {
    screenInit(1);            
    keyboardInit();           
    srand(time(NULL));        

    player.x = (MINX + MAXX) / 2;
    player.y = (MINY + MAXY) / 2;
}

void end_game() {
    keyboardDestroy();        
    screenDestroy();          
}

void handle_input() {
    if (keyhit()) {
        int key = readch();
        switch (key) {
            case 'w':
            case 'W':
                if (player.y > MINY + 1) player.y--;
                break;
            case 's':
            case 'S':
                if (player.y < MAXY - 1) player.y++;
                break;
            case 'a':
            case 'A':
                if (player.x > MINX + 1) player.x--;
                break;
            case 'd':
            case 'D':
                if (player.x < MAXX - 1) player.x++;
                break;
            case 'q':
            case 'Q':
                game_over = 1;
                break;
        }
    }
}

void spawn_monster() {
    if (monster_count < MAX_MONSTERS) {
        Entity monster;
        int side = rand() % 4;
        switch (side) {
            case 0: // Topo
                monster.x = rand() % (MAXX - MINX - 2) + MINX + 1;
                monster.y = MINY + 1;
                break;
            case 1: // Base
                monster.x = rand() % (MAXX - MINX - 2) + MINX + 1;
                monster.y = MAXY - 1;
                break;
            case 2: // Esquerda
                monster.x = MINX + 1;
                monster.y = rand() % (MAXY - MINY - 2) + MINY + 1;
                break;
            case 3: // Direita
                monster.x = MAXX - 1;
                monster.y = rand() % (MAXY - MINY - 2) + MINY + 1;
                break;
        }
        monsters[monster_count++] = monster;
    }
}

void update_monsters() {
    for (int i = 0; i < monster_count; i++) {
        if (monsters[i].x < player.x) monsters[i].x++;
        else if (monsters[i].x > player.x) monsters[i].x--;

        if (monsters[i].y < player.y) monsters[i].y++;
        else if (monsters[i].y > player.y) monsters[i].y--;
    }
}

void check_collisions() {
    for (int i = 0; i < monster_count; i++) {
        if (monsters[i].x == player.x && monsters[i].y == player.y) {
            game_over = 1;
            break;
        }
    }
}

void draw_screen() {
    static Entity prev_player = { -1, -1 };
    static Entity prev_monsters[MAX_MONSTERS];
    static int prev_monster_count = 0;

    if (prev_player.x != -1 && prev_player.y != -1) {
        screenGotoxy(prev_player.x, prev_player.y);
        printf(" ");
    }

    for (int i = 0; i < prev_monster_count; i++) {
        screenGotoxy(prev_monsters[i].x, prev_monsters[i].y);
        printf(" ");
    }

    screenGotoxy(player.x, player.y);
    printf("@");

    for (int i = 0; i < monster_count; i++) {
        screenGotoxy(monsters[i].x, monsters[i].y);
        printf("M");
    }

    prev_player = player;
    for (int i = 0; i < monster_count; i++) {
        prev_monsters[i] = monsters[i];
    }
    prev_monster_count = monster_count;

    fflush(stdout);
}

int main() {
    init_game();

    int game_tick = 100;          
    int spawn_interval = 2000;   
    int spawn_timer = 0;

    timerInit(game_tick);

    while (!game_over) {
        handle_input();
        update_monsters();
        check_collisions();

        draw_screen();

        spawn_timer += game_tick;
        if (spawn_timer >= spawn_interval) {
            spawn_monster();
            spawn_timer = 0;

            if (spawn_interval > 500) {
                spawn_interval -= 100;
            }
        }

        while (!timerTimeOver()) {
            usleep(1000); //
        }
        timerDestroy();
        timerInit(game_tick);
    }

    screenClear();
    screenDrawBorders();
    screenGotoxy((MINX + MAXX) / 2 - 5, (MINY + MAXY) / 2);
    printf("Game Over!");
    fflush(stdout);

    timerInit(2000);    
    while (!timerTimeOver()) {
        usleep(1000);
    }
    timerDestroy();

    end_game();
    return 0;
}
