//*****************************************************************************
// header:  mcu.h
// mcu:     PIC18LF4550
// author:  LL
// created: 03/2024
// project: graphicKit ver1 graphic display DOGM128 controll - example
//*****************************************************************************

#ifndef MCU_H
#define	MCU_H

//-----------------------------------------------------------------------------
// pinout

// PORT A
// RA0  LED1        OUT
// RA1  LED2        OUT
// RA2  LED3        OUT
// RA3  LED4        OUT
// RA4  LED5        OUT
// RA5  R-T         IN/OUT  TOUCH PANEL
// RA6  CLK-OUT     OUT     XTAL
// RA7  DOES NOT EXIST

// PORT B
// RB0  SW5         IN
// RB1  CLK-DSP     OUT     DISPLAY SPI
// RB2  SW3         IN
// RB3  SW4         IN
// RB4  SW2         IN
// RB5  SW1         IN
// RB6  PGC         OUT     ICSP
// RB7  PGD         OUT     ICSP

// PORT 3
// RC0  CS-DSP      OUT     DISPLAY SPI (SW)
// RC1  RST-DSP     OUT     DISPLAY RESET
// RC2  PWM-DSP     OUT     DISPLAY PWM
// RC3  DOES NOT EXIST
// RC4  USB-N       USB
// RC5  USB-P       USB
// RC6  NC
// RC7  MOSI-DSP    OUT     DISPLAY SPI

// PORT D
// RD0  A0-DSP      OUT     DISPLAY 
// RD1-RD7 NC

// PORT E
// RE0  T-T         IN/OUT  TOUCH PANEL
// RE1  L-T         IN/OUT  TOUCH PANEL
// RE2  B-T         IN/OUT  TOUCH PANEL
// RE3-RE7 DO NOT EXIST

// LEDS
#define LED1_ON     (LATA |= (1<<0))
#define LED1_OFF    (LATA &= ~(1<<0))
#define LED1_TOGGLE (LATA ^= (1<<0))

#define LED2_ON     (LATA |= (1<<1))
#define LED2_OFF    (LATA &= ~(1<<1))
#define LED2_TOGGLE (LATA ^= (1<<1))

#define LED3_ON     (LATA |= (1<<2))
#define LED3_OFF    (LATA &= ~(1<<2))
#define LED3_TOGGLE (LATA ^= (1<<2))

#define LED4_ON     (LATA |= (1<<3))
#define LED4_OFF    (LATA &= ~(1<<3))
#define LED4_TOGGLE (LATA ^= (1<<3))

#define LED5_ON     (LATA |= (1<<4))
#define LED5_OFF    (LATA &= ~(1<<4))
#define LED5_TOGGLE (LATA ^= (1<<4))

// BUTTONS
#define SW1         (PORTB & (1<<5))
#define SW2         (PORTB & (1<<4))
#define SW3         (PORTB & (1<<2))
#define SW4         (PORTB & (1<<3))
#define SW5         (PORTB & (1<<0))

uint8_t switches = 0;
volatile uint32_t millis = 0;






#endif	/* MCU_H */

