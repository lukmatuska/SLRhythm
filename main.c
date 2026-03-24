//*****************************************************************************
// source:  main.c
// author:  LL
// created: 03/2024
// project: graphicKit ver1 graphic display DOGM128 controll - example
//*****************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "mcu.h"
#include "moduleDogm128.h"

struct tile{
    uint32_t start; //start time of the individual tile
    uint16_t len;  //lenth of the tile in time
};


void setInterrupt(void);
void setPinout(void);
void clearLeds(void);
void setTimer0(void);
void setTimer2(void);
uint8_t switches = 0;

void hadleSwitches(void);
void drawUi(void);

struct tile Col1[10];
struct tile Col2[10];
struct tile Col3[10];
struct tile Col4[10];

uint8_t Col1cnt = 0;
uint8_t Col2cnt = 0;
uint8_t Col3cnt = 0;
uint8_t Col4cnt = 0;

uint8_t Col1inc = 0;
uint8_t Col2inc = 0;
uint8_t Col3inc = 0;
uint8_t Col4inc = 0;

uint16_t score = 0;

char* utoa32(uint32_t value, char* buffer);

char DispCtrStr[5] = "asdd";
int cnt = 0;
volatile uint32_t millis = 0;




void handleSwitches(void){
    if((millis - Col1[0].start - 6300) <=8000 && (millis - Col1[0].start - 6300) <=Col1[0].len) {
            score++;
        }
    if (!PORTBbits.RB5){
        switches |= (1 << 0);
    } else {
        switches &= ~(1 << 0);
    }
    if (!PORTBbits.RB4){
        switches |= (1 << 1);
    } else {
        switches &= ~(1 << 1);
    }
    if (!PORTBbits.RB3){
        switches |= (1 << 2);
    } else {
        switches &= ~(1 << 2);
    }
    if (!PORTBbits.RB0){
        switches |= (1 << 3);
    } else {
        switches &= ~(1 << 3);
    }
}

void checkForActiveTiles(){
    if(Col1inc){
        Col1cnt++;
        for(uint8_t i=0; i<10; i++){
            //Col1[i] = bigArray[i+Collcnt];
        }
    }
    if(Col2inc){
        Col1cnt++;
        for(uint8_t i=0; i<10; i++){
            //Col2[i] = bigArray[i+Collcnt];
        }
    }
    if(Col3inc){
        Col1cnt++;
        for(uint8_t i=0; i<10; i++){
            //Col3[i] = bigArray[i+Collcnt];
        }
    }
    if(Col4inc){
        Col1cnt++;
        for(uint8_t i=0; i<10; i++){
            //Col4[i] = bigArray[i+Collcnt];
        }
    }
}

/*
struct tile* tileInit(uint32_t start, uint16_t len, uint8_t col){
    struct tile outputTile = malloc(sizeof(struct tile));
    outputTile.col = col;
    outputTile.len = len;
    outputTile.start = start;
    return *outputTile;    
}*/

struct tile* tileInit(uint32_t start, uint16_t len){
    struct tile* outputTile = malloc(sizeof(struct tile));
    
    if (outputTile == NULL) return NULL; // good practice
    
    outputTile->len = len;
    outputTile->start = start;
    
    return outputTile;    
}

void drawColl(uint8_t x, struct tile activeCol[]){
    for(uint8_t i=0; i<10; i++){
        if(activeCol[i].len){
            int16_t y = (millis - activeCol[i].start) / 100;
            uint8_t height = activeCol[i].len / 100;

            // draw only if visible on screen
            if (y < 64 && (y + height) > 0){
                drawRect(x, y, 26, height);
            }
            // debug text
            //utoa32(y, DispCtrStr);
            //drawText(i, 0, DispCtrStr);
        }
    }
}


void drawUi(){
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
    
    drawColl(3, Col1);
    drawColl(34, Col2);
    drawColl(67, Col3);
    drawColl(99, Col4);
    
    
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
    
    //handleSwitches();

    //drawLine(0, 0, 127, 63);
    //drawRect(10, 10, 50, 30);
    //fillRect(70, 20, 30, 20);

    //updateDisplay();
    
    //fillChessBoardDisplay();
    //writeTextToDisplay(0,0, DispCtrStr);
    //writeTextToDisplay(0,0, DispCtrStr);
    /*writeTextToDisplay(1,0, "technicke v Brne");
    writeTextToDisplay(2,0, "nejoblibenejsi");
    writeTextToDisplay(3,0, "predmet je");
    writeTextToDisplay(4,0, "MPC-NEN");
    writeTextToDisplay(5,10, "pokus");
    writeTextToDisplay(6,20, "&&& example ###");
    writeTextToDisplay(7,20, "!!! do toho $$$ ");*/
    
    while(1)
    {
        if ( (uint32_t) millis%2 == 0){
            handleSwitches();
        }
        if ( (uint32_t) millis%100 == 0){
            drawUi();
            updateDisplay();
            clearBuffer();
            //clearBuffer();
            //sprintf(DispCtrStr, "%u", (uint32_t)millis / 100);
            
            //LED2_TOGGLE;
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
       
       (uint32_t) millis++;
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
void setTimer0(void)
{
    T0CON = 0xC3;           // ON, 8bit timer, fosc/4, prescaler, 1:16
    TMR0 = 0;               //
    INTCONbits.TMR0IF = 0;  // TMR0 flag is cleared
    INTCONbits.TMR0IE = 1;  // TMR0 interuupt is disabled
}

//*****************************************************************************
// set and open the timer 2 source fosc/4
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

