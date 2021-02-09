#ifndef BALL_H
#define BALL_H
// Native
#include <gb/gb.h>
// Local
#include "util.h"

typedef struct {
    UINT8 nb;
    UINT8 x, y;
    INT8 vx, vy;
    UINT8 w, h;
} Ball;

/** Gets a more accurate representation of of the Ball's x position.
 * @param ball      The Ball to calculate the virtual x for.
 * 
 * @return          The virtual x position of the Ball.
 */
UINT8 get_ball_virtual_x(Ball* ball) {
    return ball->x + ((8 - ball->w) / 2);
}

/** Gets a more accurate representation of of the Ball's y position.
 * @param ball      The Ball to calculate the virtual y for.
 * 
 * @return          The virtual y position of the Ball.
 */
UINT8 get_ball_virtual_y(Ball* ball) {
    return ball->y + ((8 - ball->h) / 2);
}

Ball ball = {
    0, // Sprite ID
    BALL_DEFAULT_X, BALL_DEFAULT_Y, // Position
    -1, -1, // Velocity
    4, 4, // Width and height
};

#endif // BALL_H