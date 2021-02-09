#ifndef PADDLE_H
#define PADDLE_H
// Native
#include <gb/gb.h>
#include <types.h>

typedef struct {
    UINT8 nb_top, nb_middle, nb_bottom; // Sprite IDs
    UINT8 x, y;                         // Position
    INT8 vy;                            // Velocity Y
    UINT8 w, h;                         // True width and height
    UINT8 collision_w, collision_h;     // Collision width and height
} Paddle;

/** Initializes a Paddle struct.
 * 
 * @param paddle        The Paddle struct to initalize
 * @param nb_top        The sprite ID for the top paddle sprite
 * @param nb_middle     The sprite ID for the middle paddle sprite
 * @param nb_bottom     The sprite ID for the bottom paddle sprite
 * @param x             The paddle's x position
 * @param y             The paddle's y position
 * @param vy            The paddle's y velocity
 * @param w             The paddle's width
 * @param h             The paddle's height
 * @param collision_w   The paddle's collision rect width
 * @param collision_h   The paddle's collision rect height
 */
void init_paddle(Paddle* paddle, UINT8 nb_top, UINT8 nb_middle, UINT8 nb_bottom, 
    UINT8 x, UINT8 y, INT8 vy, UINT8 w, UINT8 h, UINT8 collision_w, 
    UINT8 collision_h) 
{
    paddle->nb_top = nb_top;
    paddle->nb_middle = nb_middle;
    paddle->nb_bottom = nb_bottom;
    paddle->x = x;
    paddle->y = y;
    paddle->vy = vy;
    paddle->w = w;
    paddle->h = h;
    paddle->collision_w = collision_w;
    paddle->collision_h = collision_h;
}

/** Gets a more accurate representation of of the Paddle's x position.
 * 
 * @param paddle        The Paddle to calculate the virtual x for
 * @param flip          TRUE for the opponent's paddle and FALSE for the 
 *                      player's Paddle
 * 
 * @return              The virtual x position of the Paddle
 */
UINT8 get_paddle_virtual_x(Paddle* paddle, BOOLEAN flip) {
    if (flip) {
        return paddle->x;
    } else {
        return paddle->w - paddle->collision_w + paddle->x;
    }
}

/** Gets a more accurate representation of of the Paddle's x position.
 * 
 * @param paddle        The Paddle to calculate the virtual x for.
 * @param flip          TRUE for the opponent's paddle and FALSE for the 
 *                      player's Paddle.
 * 
 * @return              The virtual x position of the Paddle.
 */
UINT8 get_paddle_virtual_y(Paddle* paddle) {
    return paddle->y + ((paddle->w - paddle->collision_w) / 2u);
}
#endif // PADDLE_H
