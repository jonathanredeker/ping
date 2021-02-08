#include <gb/gb.h>
#include <gb/bgb_emu.h>
#include <types.h>
#include "include/tileset.c"
#include "include/map.c"

#define OFFSET_X 8
#define OFFSET_Y 16
#define BALL_DEFAULT_X 84
#define BALL_DEFAULT_Y 92
 
enum GameState {
    MENU,
    GAME,
    PAUSE,
    WIN,
    LOSE
};

UBYTE gameState = MENU;

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
    8u, 26u, 
    144u, 108u
};

Ball ball = {
    0,
    BALL_DEFAULT_X, BALL_DEFAULT_Y,
    -1, 0,
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
    set_bkg_data(0u, 30u, tileset);
    set_sprite_data(0u, 30u, tileset);
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

void collide_ball_goal(Ball* ball, Paddle* player, Paddle* opponent) {
    if (ball->vx < 0) {
        if (ball->x - 2 < player->x) {
            //BGB_MESSAGE("P2 Scores!");
            ball->vx *= -1;
            ball->x = BALL_DEFAULT_X;
            ball->y = BALL_DEFAULT_Y;
        }
    } else if (ball->vx > 0) {
        if (ball->x + 3 > opponent->x) {
            //BGB_MESSAGE("P1 Scores!");
            ball->vx *= -1;
            ball->x = BALL_DEFAULT_X;
            ball->y = BALL_DEFAULT_Y;
        }
    }
}

/*
    Experimental. The ball's velocity is affected by the paddle's velcocity.
*/
void collide_ball_paddle_new(Ball* ball, Paddle* paddle) {
    if (ball->x - OFFSET_X + 3u <= paddle->x) {
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

void menu(UINT8 input) {
    if (input == J_START) {
        gameState = GAME;
    }
}

void pause() {
    waitpad(J_START);
    gameState = GAME;
    delay(100);
}

void game(UINT8 input) {
    if (input == J_UP && player.y > 44u) {
        player.vy -= 2;
    }
    if (input == J_DOWN && player.y < 124u) {
        player.vy += 2;
    }

    player.y += player.vy;
    move_sprite(player.nb_top, player.x, player.y);
    move_sprite(player.nb_mid, player.x, player.y + 8u);
    move_sprite(player.nb_bot, player.x, player.y + 16u);

    if (opponent.y > ball.y && opponent.y > 44u) {
        opponent.vy -= 1;
    } else if (opponent.y < ball.y && opponent.y < 124u)  {
        opponent.vy += 1;
    } else {
        opponent.vy = 0;
    }

    opponent.y += opponent.vy;
    move_sprite(opponent.nb_top, opponent.x, opponent.y);
    move_sprite(opponent.nb_mid, opponent.x, opponent.y + 8u);
    move_sprite(opponent.nb_bot, opponent.x, opponent.y + 16u);

    // if (ball.x + ball.w - OFFSET_X + 2u >= walls.x + walls.w) {
    //     BGB_MESSAGE("P1 Scores!");
    //     ball.vx *= -1;
    //     ball.x = BALL_DEFAULT_X;
    //     ball.y = BALL_DEFAULT_Y;
    // }
    // if (ball.x - OFFSET_X + 2u <= walls.x) {
    //     BGB_MESSAGE("P2 Scores!");
    //     ball.vx *= -1;
    //     ball.x = BALL_DEFAULT_X;
    //     ball.y = BALL_DEFAULT_Y;
    // }
    if (ball.y + ball.h - OFFSET_Y + 2u >= walls.y + walls.h || ball.y - OFFSET_Y + 2u <= walls.y) {
        ball.vy *= -1;
    }

    collide_ball_goal(&ball, &player, &opponent);
    collide_ball_paddle(&ball, &player, FALSE);
    collide_ball_paddle(&ball, &opponent, TRUE);

    ball.x += ball.vx;
    ball.y += ball.vy;
    move_sprite(ball.nb, ball.x, ball.y);

    player.vy = 0;
    opponent.vy = 0;
}

void main() {
    init();
    while (TRUE) {
        switch (gameState) {
            case GAME:
                game(joypad());
                break;
            case MENU:
                menu(joypad());
                break;
            case PAUSE:
                pause();
                break;
            default:
                break;
        }
        delay(17);
    }
}