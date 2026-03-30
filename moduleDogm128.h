//*****************************************************************************
// header:  moduleDogm128.h
// author:  LL
// created: 03/2024
// project: header for graphic display DOGM128
//*****************************************************************************

#ifndef MODULEDOGM128_H
#define	MODULEDOGM128_H

//-----------------------------------------------------------------------------
// pinout of display

// DISPLAY
#define RST_DISP_H  (LATC |= (1<<1))
#define RST_DISP_L  (LATC &= ~(1<<1))
#define A0_DISP_H   (LATD |= (1<<0))
#define A0_DISP_L   (LATD &= ~(1<<0))
#define PWM_DISP_H  (LATC |= (1<<2))
#define PWM_DISP_L  (LATC &= ~(1<<2))


void initDisplay(void);
void clearAllDisplay(void);
void clearPageDisplay(unsigned char page);
void fillChessBoardDisplay(void);
void writeTextToDisplay(unsigned char page, unsigned char column, char* txt);

void drawText(uint8_t page, uint8_t column, const char* txt);
void updateDisplay(void);
void drawPixel(uint8_t x, uint8_t y, uint8_t color);
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void clearBuffer(void);


#endif	/* MODULEDOGM128_H */

