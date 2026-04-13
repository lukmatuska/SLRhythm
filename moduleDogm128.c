//*****************************************************************************
// source:  moduleDogm128.c 128x64 pixels
// project: module for graphic display DOGM128
// author:  LL
// created: 03/2024
//*****************************************************************************
#include <xc.h>
#include "moduleDogm128.h"
#include "spi.h"
#include "fontA.h"
#include "mcu.h"

// private functions
void sendCommandToDisplay(unsigned char comm);
void sendDataToDisplay(unsigned char dat);
void setColumnAddress(unsigned char column);
void setPageAddress(unsigned char page);


static uint8_t displayBuffer[8][128];

//*****************************************************************************
// initialization of display DOGM128
// 128x64 pixels, 128 columns, 8 pages (each has 8 rows)
void initDisplay(void)
{
    initSpi();
    PWM_DISP_H;     // Light full
    RST_DISP_H;     // RST inactive
    
    //sendCommandToDisplay(0xAF);
    sendCommandToDisplay(0x40); // Display start at line 0
    sendCommandToDisplay(0xA1); // ADC reverse
    sendCommandToDisplay(0xC0); // Normal COM0 - COM63
    sendCommandToDisplay(0xA6); // Display normal
    sendCommandToDisplay(0xA2); // Set Bias 1/9
    sendCommandToDisplay(0x2F); // Booster, regulator and follower on
    sendCommandToDisplay(0xF8); // Set Internal Booster to 4x
    sendCommandToDisplay(0x00);
    sendCommandToDisplay(0x27); // Voltage regulator set
    sendCommandToDisplay(0x81); // Contrast set
    sendCommandToDisplay(0x14);
    sendCommandToDisplay(0xAC); // No indicator
    sendCommandToDisplay(0x00);
    sendCommandToDisplay(0xAF); // Display on
    
    //BL_OFF;
    
    clearAllDisplay();
}

//*****************************************************************************
// clear all 8 pages
void clearAllDisplay(void)
{
    unsigned char pg; // page(8)
    for(pg=0; pg<8; pg++)
    {
        clearPageDisplay(pg);
    }
}

//*****************************************************************************
// clear one of 8 pages (0 - 7)
void clearPageDisplay(unsigned char page)
{
    int cl; // collumn (128)
    setColumnAddress(0);
    setPageAddress(page);
    for(cl=0; cl<128;cl++)
    {
        sendDataToDisplay(0x00);
    }
}

//*****************************************************************************
// fill display as a  chess board
void fillChessBoardDisplay(void)
{
    unsigned char cl, pg; // collumn (128), page(8))
    for(pg=0; pg<8; pg++)
    {
        setColumnAddress(0);
        setPageAddress(pg);
        for(cl=0; cl<128;cl++)
        {
            sendDataToDisplay(0xAA);
            sendDataToDisplay(0x55);
        }
    }
}

//*****************************************************************************
// write text to a specific page and specific column

void writeTextToDisplay(unsigned char page, unsigned char column, char* txt)
{
    unsigned char c, i, d, x;
    
    // start position    
    setColumnAddress(column);   // of column
    setPageAddress(page);       // of pages
    
    for(c=0; c<21; c++) // max 21 characters (21*6=126 < 128col)
    {
        if(txt[c] == 0)             // end of text
            break;
        
        x = txt[c]-0x20;            // index of character
        for(i=0; i<5; i++)
        {
            d = font5x7[x][i];      // data of character column
            sendDataToDisplay(d);   // write one column of character
        }
        sendDataToDisplay(0x00);    // write space after character
    }
}

//*****************************************************************************
// private functions
//*****************************************************************************
// send command (A0 = L)
void sendCommandToDisplay(unsigned char comm)
{
    A0_DISP_L;          // command
    CS_DISP_L;          // CS active in low
    __nop();            // delay 1 step
    sendByteSpi(comm);  // send command
    __nop();            // delay 1 step
    CS_DISP_H;          // CS inactive in H
}

//*****************************************************************************
// send data (A0 = H)
void sendDataToDisplay(unsigned char dat)
{
    A0_DISP_H;          // data
    CS_DISP_L;          // CS active in low
    __nop();            // delay 1 step
    sendByteSpi(dat);   // send data
    __nop();            // delay 1 step
    CS_DISP_H;          // CS inactive in H
}

//*****************************************************************************
// set addres of collumn on a page (0 - 127)
void setColumnAddress(unsigned char column)
{
    unsigned char col;
    if(column > 127)
    {
        column = 127;
    }
    col = (column & 0x0F) | 0x00;
    sendCommandToDisplay(col);
    col = (column >> 4) | 0x10;
    sendCommandToDisplay(col);
}

//*****************************************************************************
// set page (0-7)
void setPageAddress(unsigned char page)
{
    if(page > 7)
    {
        page = 7;
    }
    sendCommandToDisplay(0xB0 | page);
}

void drawText(uint8_t page, uint8_t column, const char* txt)
{
    uint8_t c, i, d, x;
    uint8_t col = column;

    if (page > 7 || column > 127) return;

    for (c = 0; txt[c] != '\0'; c++)
    {
        if (col > 122) break; // prevent overflow (5 + 1 spacing)

        x = txt[c] - 0x20; // ASCII offset

        // write 5 columns of character
        for (i = 0; i < 5; i++)
        {
            d = font5x7[x][i];
            displayBuffer[page][col++] = d;
        }

        // spacing column
        displayBuffer[page][col++] = 0x00;
    }
}
void drawSmallText(uint8_t page, uint8_t column, const char* txt)
{
    uint8_t c, i, d, x;
    uint8_t col = column;
    if (page > 5 || column > 127) return;
    for (c = 0; txt[c] != '\0'; c++)
    {
        if (col > 124) break; // prevent overflow (3 + 1 spacing)
        x = txt[c] - 0x20; // ASCII offset
        // write 5 columns of character
        for (i = 0; i < 3; i++)
        {
            d = font3x5[x][i];
            displayBuffer[page][col++] = d;
        }
        // spacing column
        displayBuffer[page][col++] = 0x00;
    }
}

void drawSmallTextXY(uint8_t x, uint8_t y, const char* txt)
{
    if (x > 127 || y > 63) return;

    uint8_t c, i, bit, page;
    uint8_t col = x;

    page = y / 8;
    bit  = y % 8;

    for (c = 0; txt[c] != '\0'; c++)
    {
        if (col > 124) break; // prevent overflow (3 + 1 spacing)

        uint8_t ch = txt[c] - 0x20;

        for (i = 0; i < 3; i++)
        {
            uint8_t d = font3x5[ch][i];

            // draw 5 vertical pixels from font column
            for (uint8_t row = 0; row < 5; row++)
            {
                if (d & (1 << row))
                {
                    uint8_t py = y + row;
                    if (py > 63) continue;

                    uint8_t p = py / 8;
                    uint8_t b = py % 8;

                    displayBuffer[p][col] |= (1 << b);
                }
            }

            col++;
        }

        // spacing column
        col++;
    }
}

void updateDisplay(void)
{
    for (uint8_t pg = 0; pg < 8; pg++)
    {
        setColumnAddress(0);
        setPageAddress(pg);

        for (uint8_t col = 0; col < 128; col++)
        {
            sendDataToDisplay(displayBuffer[pg][col]);
        }
    }
}

void drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= 128 || y >= 64) return;

    uint8_t page = y >> 3;        // y / 8
    uint8_t bit  = y & 0x07;      // y % 8

    if (color)
        displayBuffer[page][x] |= (1 << bit);
    else
        displayBuffer[page][x] &= ~(1 << bit);
}

void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1)
    {
        drawPixel(x0, y0, 1);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    // reject invalid sizes
    if (w == 0 || h == 0) return;

    // clip to screen bounds (128x64)
    if (x >= 128 || y >= 64) return;

    if (x + w > 128) w = 128 - x;
    if (y + h > 64)  h = 64 - y;

    // top & bottom
    for (uint8_t i = 0; i < w; i++)
    {
        drawPixel(x + i, y, 1);
        drawPixel(x + i, y + h - 1, 1);
    }

    // sides
    for (uint8_t i = 0; i < h; i++)
    {
        drawPixel(x, y + i, 1);
        drawPixel(x + w - 1, y + i, 1);
    }
}

void drawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    if(h >= 7 && w >= 7) {
        for (uint8_t i = 2; i < w; i++)
    {
        drawPixel(x + i, y, 1);
        drawPixel(x + i, y + h - 1, 1);
    }

    // sides
    for (uint8_t i = 2; i < h; i++)
    {
        drawPixel(x, y + i, 1);
        drawPixel(x + w - 1, y + i, 1);
    }
    //corners
    drawPixel(x+1, y+1, 1);
    drawPixel(x+1, y+2, 1);
    drawPixel(x+2, y+1, 1);

    drawPixel(x+w-1, y+1, 1);
    drawPixel(x+w-1, y+2, 1);
    drawPixel(x+w-2, y+1, 1);

    drawPixel(x+w-1, y+h-1, 1);
    drawPixel(x+w-1, y+h-2, 1);
    drawPixel(x+w-2, y+h-1, 1);

    drawPixel(x+1, y+h-1, 1);
    drawPixel(x+1, y+h-2, 1);
    drawPixel(x+2, y+h-1, 1);
    
    } else {
        drawRect(x, y, w, h);
    }
}

void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
    
    if(h>=2 && w>=2 ){
        for (uint8_t i = 0; i < h; i++)
        {
            for (uint8_t j = 0; j < w; j++)
            {
                drawPixel(x + j, y + i, color);
            }
        }
    }
    //drawRect(x, y, w, h);
}

void clearBuffer(void)
{
    for (uint8_t pg = 0; pg < 8; pg++)
    {
        for (uint8_t col = 0; col < 128; col++)
        {
            displayBuffer[pg][col] = 0x00;
        }
    }
}

//*****************************************************************************
// Draw a bitmap from program memory at pixel position (x, y).
//
// Bitmap format (row-major, big-endian):
//   - rows are stored top to bottom
//   - each row occupies ceil(width/8) bytes
//   - within each byte the MSB is the leftmost pixel
//   - rows are padded to a full byte boundary (unused LSBs are 0)
//
// Example: a 16x16 image ? 2 bytes/row × 16 rows = 32 bytes
//          a 12x8  image ? 2 bytes/row × 8  rows = 16 bytes (4 bits wasted/row)
//*****************************************************************************
void drawBitmap(uint8_t x, uint8_t y, const Bitmap *bmp)
{
    if (!bmp || !bmp->data)         return;
    if (x >= 128 || y >= 64)        return;

    uint8_t bytesPerRow = (bmp->width + 7) / 8; // ceil(width / 8)

    for (uint8_t row = 0; row < bmp->height; row++)
    {
        uint8_t py = y + row;
        if (py >= 64) break;                     // clip bottom

        for (uint8_t byteIdx = 0; byteIdx < bytesPerRow; byteIdx++)
        {
            uint8_t octet = bmp->data[row * bytesPerRow + byteIdx];

            for (uint8_t bit = 0; bit < 8; bit++)
            {
                // MSB of the byte ? leftmost pixel in this group of 8
                uint8_t px = x + byteIdx * 8 + bit;
                if (px >= 128) break;            // clip right

                uint8_t on = (octet & (0x80 >> bit)) ? 1 : 0;
                drawPixel(px, py, on);
            }
        }
    }
}