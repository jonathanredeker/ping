#ifndef PADDLE_H
#define PADDLE_H
// Native
#include <gb/gb.h>
#include <types.h>

typedef struct {
    UINT8 nb_top, nb_mid, nb_bot;
    UINT8 x, y;
    INT8 vy;
    UINT8 w, h;
} Paddle;

/**
 * @param paddle    The paddle struct to calculate the virtual x for.
 * @param flip      TRUE for the opponent's paddle and FALSE for the player's paddle.
 * 
 * @return          The virtual x position of the paddle.
 */
UINT8 get_paddle_virtual_x(Paddle* paddle, BOOLEAN flip) {
    if (flip) {
        return paddle->x;
    } else {
        return paddle->w - paddle->w + paddle->x;
    }
}

Paddle player = {
    1, 2, 3, // Sprite IDs
    24, 80, // Position
    1, // Y velocity
    8, 24 // Width and height
};

Paddle opponent = {
    4, 5, 6, // Sprite IDs
    144, 80, // Position
    1, // Y velocity
    8, 24 // Width and height
};
#endif // PADDLE_H
