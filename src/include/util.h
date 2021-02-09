#ifndef UTIL_H
#define UTIL_H
// Native
#include <gb/gb.h>
#include <types.h>

#define OFFSET_X 8
#define OFFSET_Y 16
#define BALL_DEFAULT_X 84
#define BALL_DEFAULT_Y 92

BOOLEAN colliding(
    UINT8 x1, UINT8 y1, UINT8 w1, UINT8 h1,
    UINT8 x2, UINT8 y2, UINT8 w2, UINT8 h2
) {
    if (x1 + w1 >= x2 && x2 + w2 >= x1) {
        if (y1 + h1 >= y2 && y2 + h2 >= y1) {
            return TRUE;
        }
    }
    return FALSE;
}

#endif // UTIL_H