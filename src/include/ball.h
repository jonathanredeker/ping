#ifndef BALL_H
#define BALL_H
// Native
#include <gb/gb.h>
#include <gb/bgb_emu.h>
// Local
#include "util.h"

#define BALL_DEFAULT_X 84
#define BALL_DEFAULT_Y 92

typedef struct {
    UINT8 nb;                           // Sprite ID
    UINT8 x, y;                         // Position
    UINT8 last_x, last_y;               // The ball's position in the last frame
    INT8 vx, vy;                        // Velocity vector
    UINT8 w, h;                         // True width and height
    UINT8 collision_w, collision_h;     // Collision width and height
} Ball;

/** Initializes a Ball struct.
 * 
 * @param ball          The Ball struct to initalize
 * @param nb            The sprite ID
 * @param x             The ball's x position
 * @param y             The ball's y position
 * @param vx            The ball's x velocity
 * @param vy            The ball's y velocity
 * @param w             The ball's width
 * @param h             The ball's height
 * @param collision_w   The ball's collision box width
 * @param collision_h   The ball's collision box height
 */
void init_ball(Ball* ball, UINT8 nb, UINT8 x, UINT8 y, INT8 vx, INT8 vy, 
    UINT8 w, UINT8 h, UINT8 collision_w, 
    UINT8 collision_h) 
{
    ball->nb = nb;
    ball->x = x;
    ball->y = y;
    ball->last_x = x;
    ball->last_y = y;
    ball->vx = vx;
    ball->vy = vy;
    ball->w = w;
    ball->h = h;
    ball->collision_h = collision_h;
    ball->collision_w = collision_w;
}

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
#endif // BALL_H