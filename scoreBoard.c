#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include <string.h>

char *fileName = "scoreBoard.save";

ScoreBoard obtemScoreBoard(){
    ScoreBoard scoreBoard = {
            .tanqueA = 0,
            .tanqueB = 0
    };
    FILE *file;
    file = fopen(fileName, "r+");

    if(file){
        int tanqueA = 0;
        int tanqueB = 0;

        char buffer[6];
        buffer[0] = '\0';
        fseek(file, -5, SEEK_END);

        fgets(buffer, 5, file);

        if(strlen(buffer) > 0){
            tanqueA = atoi(strtok(buffer, " "));
            tanqueB = atoi(strtok(NULL , " "));
        }
        scoreBoard.tanqueA = tanqueA;
        scoreBoard.tanqueB = tanqueB;
    }

    fclose(file);

    return scoreBoard;
}

void atualizaScoreBoard(ScoreBoard scoreBoard) {
    FILE *file;
    file = fopen(fileName, "w+");

    char historyTxt[5];
    sprintf(historyTxt, "%d %d", scoreBoard.tanqueA, scoreBoard.tanqueB);
    fputs(historyTxt, file);

    fclose(file);
}