
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "mcu.h"
#include "moduleDogm128.h"
#include "gameLogic.h"
#include "level.h" 
#include "fx8.h"


void checkHit(struct tile col[], uint8_t cnt){
    for(uint8_t i=0; i<cnt; i++){
        if(col[i].len > 0){
            int32_t error = millis - col[i].start;
            if (error > -HIT_WINDOW && error < HIT_WINDOW){
                score++;
                //col[i].len = 0; // mark as hit
            } else if (error > -MISS_WINDOW && error < MISS_WINDOW){
                //add miss
                misses++;
                col[i].len = 0; // mark as miss
                ierror = error;
                continue;
            } 

        }
    }
}

void computeAcc(){
        // passed_tiles * 100 / passed_tiles - misses
    
}


void addTile(struct tile col[], uint8_t *cnt, struct tile t){
    if (*cnt >= MAX_ACTIVE) return;

    col[*cnt] = t;
    (*cnt)++;
}


void spawnTiles(void){
    while (chartIndex < CHART_SIZE &&
           chart[chartIndex].start <= millis + SPAWN_AHEAD){

        struct tile t = chart[chartIndex];

        switch(t.col){
            case 0: addTile(Col1, &Col1cnt, t); break;
            case 1: addTile(Col2, &Col2cnt, t); break;
            case 2: addTile(Col3, &Col3cnt, t); break;
            case 3: addTile(Col4, &Col4cnt, t); break;
        }

        chartIndex++;
    }
}


void updateColumn(struct tile col[], uint8_t *cnt){
    for(uint8_t i = 0; i < *cnt; i++){
        if (millis > col[i].start + col[i].len + DESPAWN_TIME){

            // shift left
            for(uint8_t j = i; j < (*cnt - 1); j++){
                col[j] = col[j + 1];
            }

            (*cnt)--;
            i--; // recheck same index
        }
    }
}

void updateTiles(void){
    updateColumn(Col1, &Col1cnt);
    updateColumn(Col2, &Col2cnt);
    updateColumn(Col3, &Col3cnt);
    updateColumn(Col4, &Col4cnt);
}

void resetGame(void)
{
    // stop interrupts (avoid race conditions)
    INTCONbits.GIE = 0;

    // reset time
    millis = 0;

    // reset chart
    chartIndex = 0;
    
    passed_tiles = 0;

    // clear tile buffers
    Col1cnt = 0;
    Col2cnt = 0;
    Col3cnt = 0;
    Col4cnt = 0;

    // zero memory
    for(uint8_t i = 0; i < MAX_ACTIVE; i++){
        Col1[i].len = 0;
        Col2[i].len = 0;
        Col3[i].len = 0;
        Col4[i].len = 0;
    }

    // reset input + score
    switches = 0;
    score = 0;

    // clear display
    clearBuffer();
    updateDisplay();

    // clear timer flags (important on PIC)
    PIR1bits.TMR2IF = 0;
    INTCONbits.TMR0IF = 0;

    // re-enable interrupts
    INTCONbits.GIE = 1;
}

struct tile* tileInit(uint32_t start, uint16_t len){
    struct tile* outputTile = malloc(sizeof(struct tile));
    
    if (outputTile == NULL) return NULL; // good practice
    
    outputTile->len = len;
    outputTile->start = start;
    
    return outputTile;    
}
