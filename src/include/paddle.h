#ifndef PADDLE_H
#define PADDLE_H
// Native
#include <gb/gb.h>
#include <types.h>

typedef struct {
    UINT8 nb_top, nb_mid, nb_bot; // Sprite IDs
    UINT8 x, y; // Position
    INT8 vy; // Velocity Y
    UINT8 w, h; // True width and height
    UINT8 collision_w, collision_h; // Collision width and height
} Paddle;

/** Gets a more accurate representation of of the Paddle's x position.
 * @param paddle    The Paddle to calculate the virtual x for.
 * @param flip      TRUE for the opponent's paddle and FALSE for the player's Paddle.
 * 
 * @return          The virtual x position of the Paddle.
 */
UINT8 get_paddle_virtual_x(Paddle* paddle, BOOLEAN flip) {
    if (flip) {
        return paddle->x;
    } else {
        return paddle->w - paddle->collision_w + paddle->x;
    }
}

/** Gets a more accurate representation of of the Paddle's x position.
 * @param paddle    The Paddle to calculate the virtual x for.
 * @param flip      TRUE for the opponent's paddle and FALSE for the player's Paddle.
 * 
 * @return          The virtual x position of the Paddle.
 */
UINT8 get_paddle_virtual_y(Paddle* paddle) {
    return paddle->y + ((paddle->w - paddle->collision_w) / 2);
}

void init_paddle(Paddle* paddle, UINT8 nb_top, UINT8 nb_mid, UINT8 nb_bot, UINT8 x, UINT8 y, INT8 vy, UINT8 w, UINT8 h, UINT8 collision_w, UINT8 collision_h) {
    paddle->nb_top = nb_top;
    paddle->nb_mid = nb_mid;
    paddle->nb_bot = nb_bot;
    paddle->x = x;
    paddle->y = y;
    paddle->vy = vy;
    paddle->w = w;
    paddle->h = h;
    paddle->collision_w = collision_w;
    paddle->collision_h = collision_h;
}

/* Paddle player = {
    1, 2, 3, // Sprite IDs
    24, 80, // Position
    1, // Velocity Y
    8, 24, // True width and height
    3, 24 // Collision width and height
}; */

/* Paddle opponent = {
    4, 5, 6, // Sprite IDs
    144, 80, // Position
    1, // Velocity Y
    8, 24, // True width and height
    3, 24 // Collision width and height
}; */
#endif // PADDLE_H
