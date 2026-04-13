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
void drawSmallText(uint8_t page, uint8_t column, const char* txt);
void drawSmallTextXY(uint8_t x, uint8_t y, const char* txt);
void updateDisplay(void);
void drawPixel(uint8_t x, uint8_t y, uint8_t color);
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void drawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void clearBuffer(void);

// Bitmap descriptor ? width and height travel with the data
typedef struct {
    uint8_t  width;
    uint8_t  height;
    const uint8_t *data; // row-major, MSB = leftmost pixel, rows padded to full bytes
} Bitmap;

void drawBitmap(uint8_t x, uint8_t y, const Bitmap *bmp);

// ---------------------------------------------------------------------------
// Example: 16x16 smiley face stored in program memory
// Each row is ceil(16/8) = 2 bytes; 16 rows ? 32 bytes total
// ---------------------------------------------------------------------------
static const uint8_t smileyData[] = {
    0x03, 0xC0,  // row  0  ......##.##......
    0x0F, 0xF0,  // row  1  ....########.....
    0x1E, 0x78,  // row  2  ...####..####....
    0x3B, 0xDC,  // row  3  ..###.####.###...  ? eye pixels
    0x7F, 0xFE,  // row  4  .##############..
    0x7F, 0xFE,  // row  5  .##############..
    0x63, 0xC6,  // row  6  .##....##....##..  ? eyes
    0x7F, 0xFE,  // row  7  .##############..
    0x7F, 0xFE,  // row  8  .##############..
    0x5E, 0x7A,  // row  9  .#.####..####.#.  ? mouth corners
    0x4F, 0xF2,  // row 10  .#..########..#.  ? mouth
    0x47, 0xE2,  // row 11  .#...######...#.
    0x3F, 0xFC,  // row 12  ..############..
    0x1F, 0xF8,  // row 13  ...##########...
    0x0F, 0xF0,  // row 14  ....########....
    0x03, 0xC0,  // row 15  ......####......
};

static const Bitmap bmpSmiley = { 16, 16, smileyData };


#endif	/* MODULEDOGM128_H */

