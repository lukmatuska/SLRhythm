#ifndef LEVEL_H
#define LEVEL_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "mcu.h"
#include "moduleDogm128.h"



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
    {4800, 1500, 0},
};



#endif