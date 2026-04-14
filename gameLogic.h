#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "fx8.h"

//main settings

#define AR 5
#define OD 7

struct tile{
    uint32_t start; //start time of the individual tile
    uint16_t len; //lenth of the tile in time
    uint8_t col;
    uint8_t tile_state;
            //0 = to be hit
            //1 = start was hit, leading
            //2 = released prematurely
            //3 = completed tile
};


uint16_t score = 0;
uint16_t misses = 0;
int16_t ierror = 0;
fx_t accuracy = 0;
extern uint16_t high_scores[3];

//#define CHART_SIZE (sizeof(chart)/sizeof(chart[0]))
extern const struct tile* current_chart;
extern uint16_t current_chart_size;;

uint16_t chartIndex = 0; //chart read index
uint16_t passed_tiles = 0;

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
//#define PIXELS_PER_SECOND 20
#define SPEED_FP  (PIXELS_PER_SECOND << 8) / 1000
#define MAX_ACTIVE 4
//#define SPAWN_AHEAD 6000  // ms before visible
#define DESPAWN_TIME 500

#define AR_MIN_MS 3000
#define AR_MAX_MS 8000

#define SPAWN_AHEAD (AR_MAX_MS - ((AR) * (AR_MAX_MS - AR_MIN_MS) / 10))

#define HIT_WINDOW_MIN 80
#define HIT_WINDOW_MAX 300

#define MISS_WINDOW_MIN 150
#define MISS_WINDOW_MAX 500

#define HIT_WINDOW  (HIT_WINDOW_MAX - ((OD) * (HIT_WINDOW_MAX - HIT_WINDOW_MIN) / 10))
#define MISS_WINDOW (MISS_WINDOW_MAX - ((OD) * (MISS_WINDOW_MAX - MISS_WINDOW_MIN) / 10))

#define BASE_SPEED 20
#define PIXELS_PER_SECOND (BASE_SPEED + (AR * 2))
//hit logic
//#define HIT_WINDOW 300
//#define MISS_WINDOW 500

#define FRAME_PERIOD 20
#define HIT_SCAN_PERIOD 5


void checkHit(struct tile col[], uint8_t cnt);
void checkRelease(struct tile col[], uint8_t cnt);
void computeAcc();
void addTile(struct tile col[], uint8_t *cnt, struct tile t);
void spawnTiles(void);
void updateColumn(struct tile col[], uint8_t *cnt);
void updateTiles(void);
void resetGame(void);
struct tile* tileInit(uint32_t start, uint16_t len);





#endif