#ifndef UTIL_H
#define UTIL_H
// Native
#include <gb/gb.h>
#include <types.h>

#define OFFSET_X 8
#define OFFSET_Y 16

enum GameState {
    MENU,
    GAME,
    PAUSE,
    WIN,
    LOSE
};

/** Checks if there's  a collision between two bodies.
 * 
 * @param a_x           Body A's x position
 * @param a_y           Body A's y position
 * @param a_w           Body A's width
 * @param a_h           Body A's height
 * @param b_x           Body B's x position
 * @param b_y           Body B's y position
 * @param b_w           Body B's width
 * @param b_h           Body B's height
 * 
 * @return TRUE if there's a collision; otherwise, return FALSE.
 */
BOOLEAN checkCollision(UINT8 a_x, UINT8 a_y, UINT8 a_w, UINT8 a_h,
    UINT8 b_x, UINT8 b_y, UINT8 b_w, UINT8 b_h) 
{
    if (a_x + a_w >= b_x && b_x + b_w >= a_x) {
        if (a_y + a_h >= b_y && b_y + b_h >= a_y) {
            return TRUE;
        }
    }
    return FALSE;
}
#endif // UTIL_H