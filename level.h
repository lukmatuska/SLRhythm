#ifndef LEVEL_H
#define LEVEL_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "mcu.h"
#include "moduleDogm128.h"
#include "gameLogic.h" 


/*const struct tile chart[] = { //store tiles in flash, one big array
    {1000, 500, 0},
    {1000, 200, 1},

    {2000, 300, 2},
    {2500, 400, 3},
    {3000, 700, 0},
    {3200, 500, 3},
    {3400, 200, 1},
    {4000, 300, 2},
    {4500, 400, 3},
    {4800, 1500, 0},
};
*/


// Mapa 1: 
static const struct tile chart1[] = { 
    {1000, 500, 0}, {2000, 300, 1}, {3000, 500, 2}, 
    {4000, 300, 3}, {5000, 700, 0}, {5500, 400, 2},
    {6500, 300, 1}, {7000, 400, 3}, {8000, 1000, 0}
};

// Mapa 2: 
static const struct tile chart2[] = {
    {1000, 200, 0}, {1500, 200, 2}, {2000, 200, 1}, 
    {2500, 200, 3}, {3000, 400, 0}, {3200, 200, 1},
    {3800, 300, 3}, {4000, 300, 2}, {4600, 500, 0},
    {5000, 200, 1}, {5200, 200, 2}, {5500, 600, 3}
};

// Mapa 3: 
static const struct tile chart3[] = {
    {500,  150, 0}, {700,  150, 1}, {900,  150, 2}, {1100, 150, 3},
    {1500, 300, 0}, {1700, 300, 2}, {2000, 200, 1}, {2200, 200, 3},
    {2600, 100, 0}, {2700, 100, 1}, {2800, 100, 2}, {2900, 100, 3},
    {3300, 800, 0}, {3300, 800, 3}, {4500, 500, 1}, {4500, 500, 2} 
};


#define CHART1_SIZE (sizeof(chart1)/sizeof(chart1[0]))
#define CHART2_SIZE (sizeof(chart2)/sizeof(chart2[0]))
#define CHART3_SIZE (sizeof(chart3)/sizeof(chart3[0]))

#endif

#endif