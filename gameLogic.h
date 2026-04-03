#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "mcu.h"
#include "moduleDogm128.h"



struct tile{
    uint32_t start; //start time of the individual tile
    uint16_t len; //lenth of the tile in time
    uint8_t col;
};


uint16_t score = 0;
uint16_t misses = 0;
int16_t ierror = 0;
uint16_t accuracy = 0;

#define CHART_SIZE (sizeof(chart)/sizeof(chart[0]))

uint16_t chartIndex = 0; //chart read index

#define MAX_ACTIVE 4

struct tile Col1[MAX_ACTIVE];
struct tile Col2[MAX_ACTIVE];
struct tile Col3[MAX_ACTIVE];
struct tile Col4[MAX_ACTIVE];

uint8_t Col1cnt = 0;
uint8_t Col2cnt = 0;
uint8_t Col3cnt = 0;
uint8_t Col4cnt = 0;

//tile render logic
#define PIXELS_PER_SECOND 10
#define SPEED_FP  (PIXELS_PER_SECOND << 8) / 1000
#define MAX_ACTIVE 4
#define SPAWN_AHEAD 6000  // ms before visible
#define DESPAWN_TIME 500

//hit logic
#define HIT_WINDOW 300
#define MISS_WINDOW 500

#define FRAME_PERIOD 20
#define HIT_SCAN_PERIOD 5


void checkHit(struct tile col[], uint8_t cnt);
void computeAcc();
void addTile(struct tile col[], uint8_t *cnt, struct tile t);
void spawnTiles(void);
void updateColumn(struct tile col[], uint8_t *cnt);
void updateTiles(void);
void resetGame(void);
struct tile* tileInit(uint32_t start, uint16_t len);





#endif