#ifndef RECT_H
#define RECT_H
// Native
#include <gb/gb.h>
// Local
#include "util.h"

typedef struct {
    UINT8 x, y;
    UINT8 w, h;
} Rect;

/** Initializes a Rect struct.
 * 
 * @param x             The rect's x position
 * @param y             The rect's y position
 * @param w             The rect's width
 * @param h             The rect's height
 */
void init_rect(Rect* rect, UINT8 x, UINT8 y, UINT8 w, UINT8 h) {
    rect->x = x + OFFSET_X;
    rect->y = y + OFFSET_Y;
    rect->w = w;
    rect->h = h;
}
#endif // RECT_H
