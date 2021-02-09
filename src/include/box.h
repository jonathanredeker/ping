#ifndef BOX_H
#define BOX_H
// Native
#include <gb/gb.h>
// Local
#include "util.h"

typedef struct {
    UINT8 x, y;
    UINT8 w, h;
} Box;

Box bounds = {
    8u + OFFSET_X, 24u + OFFSET_Y, 
    144u, 112u
};
#endif // BOX_H
