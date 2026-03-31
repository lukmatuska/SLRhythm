//*****************************************************************************
// source:  main.c
// author:  LM, JS
// created: 03/2026
// project: SLRhythm
//*****************************************************************************
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


void setInterrupt(void);
void setPinout(void);
void clearLeds(void);
void setTimer0(void);
void setTimer2(void);
uint8_t switches = 0;

void hadleSwitches(void);
void drawUi(void);


uint16_t score = 0;
uint16_t misses = 0;
int16_t ierror = 0;
uint16_t accuracy = 0;


char* utoa32(uint32_t value, char* buffer);

char DispCtrStr[5] = "asdd";
int cnt = 0;
volatile uint32_t millis = 0;

const struct tile chart[] = { //store tiles in flash, one big array
    {1000, 500, 0},
    {1500, 200, 1},
    {2000, 300, 2},
    {2500, 400, 3},
    {3000, 700, 0},
    {3200, 500, 3},
    {3400, 200, 1},
    {4000, 300, 2},
    {4500, 400, 3},
    {4800, 700, 0},
};

#define CHART_SIZE (sizeof(chart)/sizeof(chart[0]))
//then do streaming

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

#define PIXELS_PER_SECOND 10
#define SPEED_FP  (PIXELS_PER_SECOND << 8) / 1000
#define MAX_ACTIVE 4
#define SPAWN_AHEAD 6000  // ms before visible
#define DESPAWN_TIME 500

struct tile Coll1[MAX_ACTIVE];

#define HIT_WINDOW 300
#define MISS_WINDOW 500

void checkHit(struct tile col[], uint8_t cnt){
    for(uint8_t i=0; i<cnt; i++){
        if(col[i].len > 0){
            int32_t error = millis - col[i].start;
            if (error > -MISS_WINDOW && error < MISS_WINDOW && !(error > -HIT_WINDOW && error < HIT_WINDOW)){
                //add miss
                misses++;
                col[i].len = 0; // mark as miss
                ierror = error;
                continue;
            }

            if (error > -HIT_WINDOW && error < HIT_WINDOW){
                score++;
                //col[i].len = 0; // mark as hit
            }
        }
    }
}

void computeAcc(){
    
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


void handleSwitches(void){

    if (!PORTBbits.RB2){
        resetGame();
    }   
    
    if (!PORTBbits.RB5){
        switches |= (1 << 0);
        checkHit(Col1, Col1cnt);
    } else {
        switches &= ~(1 << 0);
    }
    if (!PORTBbits.RB4){
        switches |= (1 << 1);
        checkHit(Col2, Col1cnt);
    } else {
        switches &= ~(1 << 1);
    }
    if (!PORTBbits.RB3){
        switches |= (1 << 2);
        checkHit(Col3, Col1cnt);
    } else {
        switches &= ~(1 << 2);
    }
    if (!PORTBbits.RB0){
        switches |= (1 << 3);
        checkHit(Col4, Col1cnt);
    } else {
        switches &= ~(1 << 3);
    }
    
}

struct tile* tileInit(uint32_t start, uint16_t len){
    struct tile* outputTile = malloc(sizeof(struct tile));
    
    if (outputTile == NULL) return NULL; // good practice
    
    outputTile->len = len;
    outputTile->start = start;
    
    return outputTile;    
}
void drawColl(uint8_t x, struct tile activeCol[], uint8_t cnt){
    for(uint8_t i=0; i<cnt; i++){
        if(activeCol[i].len == 0) continue;

        int32_t dt = millis - activeCol[i].start;

        // position in pixels
        int16_t y = (dt * PIXELS_PER_SECOND) / 1000 + 60;
        //int16_t y = (dt * SPEED_FP) >> 8;
        

        // scale tile length to pixels
        uint8_t height = (activeCol[i].len * PIXELS_PER_SECOND) / 1000;

        // draw only if visible
        if (y < 64 && (y + height) > 0){
            drawRect(x, y, 26, height);
        }
    }
}

void drawButtons(){
    if (switches & 0x01){
        drawRect(2, 60, 28, 4);
        fillRect(3, 61, 26, 2, 1);
    } else {
        drawRect(2, 60, 28, 4);
        fillRect(3, 61, 26, 2, 0);
    }
    if (switches & 0x02){
        drawRect(34, 60, 28, 4);
        fillRect(35, 61, 26, 2, 1);
    } else {
        drawRect(34, 60, 28, 4);
        fillRect(35, 61, 26, 2, 0);
    }
    if (switches & 0x04){
        drawRect(66, 60, 28, 4);
        fillRect(67, 61, 26, 2, 1);
    } else {
        drawRect(66, 60, 28, 4);
        fillRect(67, 61, 26, 2, 0);
    }    
    if (switches & 0x08){
        drawRect(98, 60, 28, 4);
        fillRect(99, 61, 26, 2, 1);
    } else {
        drawRect(98, 60, 28, 4);
        fillRect(99, 61, 26, 2, 0);
    }
}

void drawBoard(){
   // drawLine();
    drawRect(0, 0, 128, 10);
    drawRect(0, 10, 2, 50);
    drawRect(32, 10, 2, 50);
    drawRect(64, 10, 2, 50);
    drawRect(96, 10, 2, 50);
    drawRect(126, 10, 2, 50);
}

void drawUi(){
    
    drawButtons();
    
    drawColl(3, Col1, Col1cnt);
    drawColl(34, Col2, Col2cnt);
    drawColl(67, Col3, Col3cnt);
    drawColl(99, Col4, Col4cnt);
    
    
    utoa32(score, DispCtrStr);
    drawText(0,0, DispCtrStr);
}



void main() 
{
    setPinout();
    clearLeds();
    setTimer0();
    setTimer2();
    
    initDisplay();
    setInterrupt();
    
    
    clearBuffer();
    Col4[0] = *tileInit(1000, 500);
    Col4[1] = *tileInit(2000, 100);
    Col4[2] = *tileInit(3000, 700);
    
    
    while(1) //main loop
    {
        if ( (uint32_t) millis%2 == 0){ //every 2ms
            handleSwitches();
        }
        if ( (uint32_t) millis%100 == 0){ //every 100ms
            spawnTiles();   // NEW
            updateTiles();  // NEW
            drawUi();
            updateDisplay();
            clearBuffer();
            //clearBuffer();
            //sprintf(DispCtrStr, "%u", (uint32_t)millis / 100);
        }
  
    }
}

//*****************************************************************************
// interrupt
void __interrupt() isr(void)
{
    // of Timer 0
    if(TMR0IF && TMR0IE)
    {
        
        TMR0IF = 0;
    }
    // of Timer 2
    if(TMR2IF && TMR2IE)
    {
       
       millis++;
        TMR2IF = 0;
    }
    // other interrupts...    
}

//*****************************************************************************
// global + peripherial interrupt
void setInterrupt(void)
{
    INTCONbits.PEIE = 1;    // enable peripheries
    INTCONbits.GIE = 1;     // enable globale interrupt
}

//*****************************************************************************
// pinout of MCU for GRAPHIC KIT 1
void setPinout(void)
{
    TRISA = 0;      //
    TRISB = 0x3D;   // RB0,2,3,4,5 are buttons (inputs) other outputs
    TRISC = 0;      //
    TRISD = 0;      //
    TRISE = 0;      //
}

//*****************************************************************************
//
void clearLeds(void)
{
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
    LED5_OFF;
}

//*****************************************************************************
// set and open the timer 0
// not used, maybe i should not initialize it to save performance
void setTimer0(void)
{
    T0CON = 0xC3;           // ON, 8bit timer, fosc/4, prescaler, 1:16
    TMR0 = 0;               //
    INTCONbits.TMR0IF = 0;  // TMR0 flag is cleared
    INTCONbits.TMR0IE = 1;  // TMR0 interuupt is disabled
}

//*****************************************************************************
//This timer is used for measuring milliseconds
//it triggers its interrupt (TMR2IE) independently on main loop
//it increments the millis value
//set and open the timer 2 source fosc/4
//millis timer, do not touch
void setTimer2(void)                        //millis timer, do not touch
{
    T2CON = 0x1F;           // ON, Prescaler 1:16, Postscaler 1:1
    PR2 = 187;              //
    PIR1bits.TMR2IF = 0;    //
    PIE1bits.TMR2IE = 1;    //
}

char* utoa32(uint32_t value, char* buffer)
{
    char temp[10]; // max 10 digits for uint32_t
    int i = 0;

    // Handle 0 explicitly
    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    // Convert to reversed string
    while (value > 0)
    {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }

    // Reverse into buffer
    int j = 0;
    while (i > 0)
    {
        buffer[j++] = temp[--i];
    }

    buffer[j] = '\0';

    return buffer;
}

