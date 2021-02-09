#ifndef BALL_H
#define BALL_H
// Native
#include <gb/gb.h>
// Local
#include "util.h"

#define BALL_DEFAULT_X 84
#define BALL_DEFAULT_Y 92

typedef struct {
    UINT8 nb; // Sprite ID
    UINT8 x, y; // Position
    UINT8 last_x, last_y; // The ball's position in the last frame
    INT8 vx, vy; // Velocity vector
    UINT8 w, h; // True width and height
    UINT8 collision_w, collision_h; // Collision width and height
} Ball;

/** Gets a more accurate representation of of the Ball's x position.
 * @param ball      The Ball to calculate the virtual x for.
 * 
 * @return          The virtual x position of the Ball.
 */
UINT8 get_ball_virtual_x(Ball* ball) {
    return ball->x + ((ball->w - ball->collision_w) / 2);
}

/** Gets a more accurate representation of of the Ball's y position.
 * @param ball      The Ball to calculate the virtual y for.
 * 
 * @return          The virtual y position of the Ball.
 */
UINT8 get_ball_virtual_y(Ball* ball) {
    return ball->y + ((ball->h - ball->collision_h) / 2);
}

Ball ball = {
    0, // Sprite ID
    BALL_DEFAULT_X, BALL_DEFAULT_Y, // Position
    0, 0,
    -1, 0, // Velocity vector
    8, 8, // True width and height
    4, 4, // Collision width and height
};

#endif // BALL_H