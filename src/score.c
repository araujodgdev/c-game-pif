#include <stdio.h> 
#include <stdlib.h>

#include "score.h"
#include "screen.h"
#include "enemy.h"
#include "keyboard.h"

void saveScore(int score)
{
    FILE *file = fopen(SCOREBOARD_FILE, "a");
    fprintf(file, "%d\n", score);
    fclose(file);
}


void printScore(int score)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(MINX + 2, MINY);
    printf(" Pontuação: %d ", score);
    fflush(stdout); 
}

void viewScoreboard()
{
    FILE *file = fopen(SCOREBOARD_FILE, "r");
    if (file == NULL)
    {
        screenClear();
        screenSetColor(LIGHTRED, DARKGRAY);
        screenGotoxy(30, 12);
        printf("Nenhum placar registrado ainda.\n");
        screenSetColor(LIGHTGRAY, DARKGRAY);
        screenGotoxy(30, 14);
        printf("Pressione qualquer tecla para voltar ao menu.");
        screenUpdate();

        while(!keyhit());

        readch();
        return;
    }
    int scores[MAX_ENEMIES];
    int count = 0;
    while(fscanf(file, "%d", &scores[count]) == 1 && count < MAX_ENEMIES)
    {
        count++;
    }
    fclose(file);

    if(count == 0)
    {
        screenClear();
        screenSetColor(LIGHTRED, DARKGRAY);
        screenGotoxy(30, 12);
        printf("Nenhum placar registrado ainda.\n");
        screenSetColor(LIGHTGRAY, DARKGRAY);
        screenGotoxy(30, 14);
        printf("Pressione qualquer tecla para voltar ao menu.");
        screenUpdate();

        while(!keyhit());

        readch();
        return;
    }

    for(int i = 0; i < count -1; i++)
    {
        for(int j = i +1; j < count; j++)
        {
            if(scores[j] > scores[i])
            {
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    screenClear();
    screenSetColor(LIGHTBLUE, DARKGRAY);
    screenGotoxy(30, 10);
    printf("=== PLACAR ===\n\n");

    screenSetColor(YELLOW, DARKGRAY);
    int displayCount = count < 10 ? count : 10;
    for(int i = 0; i < displayCount; i++)
    {
        screenGotoxy(30, 12 + i);
        printf("%d. %d", i +1, scores[i]);
    }

    screenSetColor(LIGHTGRAY, DARKGRAY);
    screenGotoxy(30, 23);
    printf("Pressione qualquer tecla para voltar ao menu.");
    screenUpdate();
    while(!keyhit());

    readch(); 
}