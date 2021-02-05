#include <gb/gb.h>
#include <gb/bgb_emu.h>
#include <types.h>
#include "include/tileset.c"
#include "include/map.c"

#define OFFSET_X 8
#define OFFSET_Y 16

typedef struct {
    UINT8 nb_top, nb_mid, nb_bot;
    UINT8 x, y;
    INT8 vy;
    UINT8 w, h;
} Paddle;

typedef struct {
    UINT8 nb;
    UINT8 x, y;
    INT8 vx, vy;
    UINT8 w, h;
    BOOLEAN colliding;
} Ball;

typedef struct {
    UINT8 x, y;
    UINT8 w, h;
} Box;

Box walls = {
    8u, 8u, 
    144u, 128u
};

Ball ball = {
    0,
    80, 72,
    1, 1,
    4, 4,
    FALSE
};

Paddle player = {
    1, 2, 3,
    24, 80,
    1,
    8, 24
};

Paddle opponent = {
    4, 5, 6,
    144, 80,
    1,
    8, 24
};

void init() {
    set_bkg_data(0u, 10u, tileset);
    set_sprite_data(0u, 10u, tileset);
    set_bkg_tiles(0u, 0u, 20u, 18u, map);

    set_sprite_tile(player.nb_top, 2u);
    set_sprite_tile(player.nb_mid, 3u);
    set_sprite_tile(player.nb_bot, 4u);
    move_sprite(player.nb_top, player.x, player.y);
    move_sprite(player.nb_mid, player.x, player.y + 8u);
    move_sprite(player.nb_bot, player.x, player.y + 16u);

    set_sprite_tile(opponent.nb_top, 6u);
    set_sprite_tile(opponent.nb_mid, 7u);
    set_sprite_tile(opponent.nb_bot, 8u);
    move_sprite(opponent.nb_top, opponent.x, opponent.y);
    move_sprite(opponent.nb_mid, opponent.x, opponent.y + 8u);
    move_sprite(opponent.nb_bot, opponent.x, opponent.y + 16u);

    set_sprite_tile(ball.nb, 5u);
    move_sprite(ball.nb, ball.x, ball.y);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}

void collide_ball_paddle(Ball* ball, Paddle* paddle, BOOLEAN flip) {
    UINT8 leftTerm = ball->x - OFFSET_X + 2u;
    UINT8 rightTerm = paddle->x;
    if (flip) {
        UINT8 tmp = leftTerm;
        leftTerm = rightTerm - 12;
        rightTerm = tmp;
    }
    if (leftTerm <= rightTerm) {
        if (ball->y - OFFSET_Y + 2u > paddle->y - 20u && ball->y + ball->h - OFFSET_Y + 1u < paddle->y + 11u) {
            if (ball->y + ball->h - OFFSET_Y + 1u < paddle->y - 8u) {
                ball->vy = -1;
            } else if (ball->y - OFFSET_Y + 2u > paddle->y) {
                ball->vy = 1;
            } else {
                ball->vy = 0;
            }
            ball->vx *= -1;
        }
    }
}

/*
    Experimental. The ball's velocity is affected by the paddle's velcocity.
*/
void collide_ball_paddle_new(Ball* ball, Paddle* paddle) {
    if (ball->x - OFFSET_X + 2u <= paddle->x) {
        if (ball->y - OFFSET_Y + 2u > paddle->y - 20u && ball->y + ball->h - OFFSET_Y + 1u < paddle->y + 11u) {
            if (paddle->vy > 0) {
                ball->vy += 1;
            } else if (paddle->vy < 0) {
                ball->vy -= 1;
            } else {
                ball->vy = 0;
            }
            ball->vx *= -1;
        }
    }
}

void main() {
    init();
    
    while (1) {
        UINT8 input = joypad();

        if (input == J_UP && player.y > 26u) {
            player.vy -= 2;
        }
        if (input == J_DOWN && player.y < 126u) {
            player.vy += 2;
        }

        player.y += player.vy;
        move_sprite(player.nb_top, player.x, player.y);
        move_sprite(player.nb_mid, player.x, player.y + 8u);
        move_sprite(player.nb_bot, player.x, player.y + 16u);

        if (opponent.y > ball.y && opponent.y > 26u) {
            opponent.vy -= 1;
        } else if (opponent.y < ball.y && opponent.y < 126u)  {
            opponent.vy += 1;
        } else {
            opponent.vy = 0;
        }

        opponent.y += opponent.vy;
        move_sprite(opponent.nb_top, opponent.x, opponent.y);
        move_sprite(opponent.nb_mid, opponent.x, opponent.y + 8u);
        move_sprite(opponent.nb_bot, opponent.x, opponent.y + 16u);

        if (ball.x + ball.w - OFFSET_X + 2u >= walls.x + walls.w || ball.x - OFFSET_X + 2u <= walls.x) {
            ball.vx *= -1;
        }
        if (ball.y + ball.h - OFFSET_Y + 2u >= walls.y + walls.h || ball.y - OFFSET_Y + 2u <= walls.y) {
            ball.vy *= -1;
        }

        collide_ball_paddle(&ball, &player, FALSE);
        collide_ball_paddle(&ball, &opponent, TRUE);

        ball.x += ball.vx;
        ball.y += ball.vy;
        move_sprite(ball.nb, ball.x, ball.y);

        player.vy = 0;
        opponent.vy = 0;
        delay(17);
    }
}