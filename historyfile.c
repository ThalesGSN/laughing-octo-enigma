#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include <string.h>

const char *fileName = "history.bin";

FILE* openFile(char* name, char* mode){
    FILE *fp;
    fp  = fopen(name, mode);

    return fp;
}


GameHistory readGameHistory(FILE* file){
    int firstPlayerWins = 0;
    int secondPlayerWins = 0;

    char buffer[6];
    buffer[0] = '\0';

    fgets(buffer, 5, file);

    if(strlen(buffer) > 0){
        firstPlayerWins = atoi(strtok(buffer," "));
        secondPlayerWins = atoi(strtok(NULL ," "));
    }

    GameHistory history = {
            .firstPlayerWins = firstPlayerWins,
            .secondPlayerWins = secondPlayerWins
    };

    return history;
}

void writeGameHistory(FILE* file, GameHistory newHistory){
    char history[5];
    sprintf(history, "%d %d", newHistory.firstPlayerWins, newHistory.secondPlayerWins);
    fputs(history, file);
}

GameHistory getGameHistory(){
    GameHistory history = {
            .firstPlayerWins = 0,
            .secondPlayerWins = 0
    };
    FILE* file = openFile(fileName, "r");

    if(file){
        history = readGameHistory(file);
    }

    fclose(file);

    return history;
}

void updateGameHistory(GameHistory history) {
    FILE* file = openFile(fileName, "w+");

    writeGameHistory(file, history);

    fclose(file);
}