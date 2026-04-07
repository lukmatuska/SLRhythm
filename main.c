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
#include "gameLogic.h"
#include "level.h"
#include "fx8.h"


void setInterrupt(void);
void setPinout(void);
void clearLeds(void);
void setTimer0(void);
void setTimer2(void);


void hadleSwitches(void);
void drawUi(void);
void drawBorders(void);


char* utoa32(uint32_t value, char* buffer);
void fx_to_str(fx_t x, char *buf);

char DispCtrStr[5] = "asdd";
int cnt = 0;



void handleSwitches(void){

    if (!PORTBbits.RB2){
        resetGame();
    }   
    
    if (!PORTBbits.RB5){
        switches |= (1 << 0);
        checkHit(Col1, Col1cnt);
        LED1_ON;
    } else {
        switches &= ~(1 << 0);
        checkRelease(Col1, Col1cnt);
        LED1_OFF;
    }
    if (!PORTBbits.RB4){
        switches |= (1 << 1);
        checkHit(Col2, Col1cnt);
        LED2_ON;
    } else {
        switches &= ~(1 << 1);
        checkRelease(Col2, Col2cnt);
        LED2_OFF;
    }
    if (!PORTBbits.RB3){
        switches |= (1 << 2);
        checkHit(Col3, Col1cnt);
        LED4_ON;
    } else {
        switches &= ~(1 << 2);
        checkRelease(Col3, Col3cnt);
        LED4_OFF;
    }
    if (!PORTBbits.RB0){
        switches |= (1 << 3);
        checkHit(Col4, Col1cnt);
        LED5_ON;
    } else {
        switches &= ~(1 << 3);
        checkRelease(Col4, Col4cnt);
        LED5_OFF;
    }
    
}

void drawColl(uint8_t x, struct tile activeCol[], uint8_t cnt){
    for(uint8_t i=0; i<cnt; i++){
        if(activeCol[i].len == 0) continue;

        int32_t dt = millis - activeCol[i].start;

        // position in pixels (now represents bottom of tile)
        int16_t y = (dt * PIXELS_PER_SECOND) / 1000 + 60;

        // scale tile length to pixels
        uint8_t height = (activeCol[i].len * PIXELS_PER_SECOND) / 1000;

        // draw only if visible
        if (y > 0 && (y - height) < 64){
            drawRoundRect(x, y - height, 26, height);
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

void drawBorders(void){
    drawRect(0, 1, 128, 63);
    drawLine(0, 6, 127, 6);
    //drawLine
}

void drawUi(){
    
    drawButtons(); //self explanatory
    
    //draw collumns of tiles
    drawColl(3, Col1, Col1cnt);
    drawColl(34, Col2, Col2cnt);
    drawColl(67, Col3, Col3cnt);
    drawColl(99, Col4, Col4cnt);
    
    utoa32(misses, DispCtrStr);
    drawSmallText(2,100, DispCtrStr);

    utoa32(passed_tiles, DispCtrStr);
    drawSmallText(1,100, DispCtrStr);
    
           
    //draw score    
    utoa32(score, DispCtrStr);
    drawSmallText(0,75, "SCORE:");
    drawSmallText(0,100, DispCtrStr);
    
    
    //draw acc
    fx_to_str(accuracy, DispCtrStr);
    //utoa32(accuracy, DispCtrStr);
    drawSmallText(0,0, "ACC:");
    drawSmallText(0,15, DispCtrStr);
    //draw borders
    drawBorders();
}



void main() 
{
    setPinout();
    clearLeds();
    setTimer0();
    setTimer2();
    
    initDisplay();
    BL_ON;
    setInterrupt();

    
    
    clearBuffer();
    Col4[0] = *tileInit(1000, 500);
    Col4[1] = *tileInit(2000, 100);
    Col4[2] = *tileInit(3000, 700);
    
    
    while(1) //main loop
    {
        if ( (uint32_t) millis % HIT_SCAN_PERIOD == 0){ //every 2ms
            handleSwitches();
            LED3_OFF;
        }
        if ( (uint32_t) millis % FRAME_PERIOD == 0){ //render loop
            updateDisplay();
            clearBuffer();
            computeAcc();
            spawnTiles();   // NEW
            updateTiles();  // NEW
            drawUi();
            //LED3_TOGGLE;
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

void fx_to_str(fx_t x, char *buf)
{
    char *p = buf;

    // handle sign
    if (x < 0) {
        *p++ = '-';
        x = -x;
    }

    // integer part
    int16_t ip = FX_TO_INT(x);

    // fractional part (0?255)
    uint8_t frac = FX_FRAC(x);

    // --- convert integer (reverse) ---
    char tmp[6]; // enough for int16
    int i = 0;

    if (ip == 0) {
        tmp[i++] = '0';
    } else {
        while (ip > 0) {
            tmp[i++] = '0' + (ip % 10);
            ip /= 10;
        }
    }

    // write integer in correct order
    while (i--) {
        *p++ = tmp[i];
    }

    // decimal point
    *p++ = '.';

    // --- fractional: scale to 2 decimal digits ---
    // frac is /256 ? convert to /100
    // (frac * 100) / 256
    uint16_t f = ((uint16_t)frac * 100) >> 8;

    // two digits
    *p++ = '0' + (f / 10);
    *p++ = '0' + (f % 10);

    *p = '\0';
}