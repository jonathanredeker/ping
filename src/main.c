// Native
#include <types.h>
#include <stdio.h>
#include <gb/gb.h>
#include <gb/bgb_emu.h>
#include <gb/drawing.h>
// Local
#include "include/tileset.c"
#include "include/map.c"
#include "include/paddle.h"
#include "include/ball.h"
#include "include/box.h"
#include "include/util.h"

char debugBuffer[100];
 
enum GameState {
    MENU,
    GAME,
    PAUSE,
    WIN,
    LOSE
};

UBYTE gameState = MENU;

void menu() {
    UINT8 input = joypad();
    if (input == J_START) {
        gameState = GAME;
    }
}

void pause() {
    waitpad(J_START);
    gameState = GAME;
    delay(100);
}

void game() {
    // Always setup background
    set_bkg_tiles(0u, 0u, 20u, 18u, map);

    // Setup player paddle sprites
    set_sprite_tile(player.nb_top, 2u);
    set_sprite_tile(player.nb_mid, 3u);
    set_sprite_tile(player.nb_bot, 4u);
    move_sprite(player.nb_top, player.x, player.y);
    move_sprite(player.nb_mid, player.x, player.y + 8u);
    move_sprite(player.nb_bot, player.x, player.y + 16u);

    // Setup opponent paddle sprites
    set_sprite_tile(opponent.nb_top, 6u);
    set_sprite_tile(opponent.nb_mid, 7u);
    set_sprite_tile(opponent.nb_bot, 8u);
    move_sprite(opponent.nb_top, opponent.x, opponent.y);
    move_sprite(opponent.nb_mid, opponent.x, opponent.y + 8u);
    move_sprite(opponent.nb_bot, opponent.x, opponent.y + 16u);

    // Setup ball sprite
    set_sprite_tile(ball.nb, 5u);
    move_sprite(ball.nb, ball.x, ball.y);

    while (TRUE) {
        UINT8 input = joypad();

        // Player movement
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

        // Opponent AI movement
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

        // Paddle and ball collision detection
        

        // Bounds and ball collision detection
        if (get_ball_virtual_y(&ball) <= bounds.y || get_ball_virtual_y(&ball) + ball.h >= bounds.y + bounds.h) {
            ball.vy *= -1;
        }
        if (get_ball_virtual_x(&ball) <= bounds.x || get_ball_virtual_x(&ball) + ball.w >= bounds.x + bounds.w) {
            ball.vx *= -1;
            // Goal
        }

        ball.x += ball.vx;
        ball.y += ball.vy;
        move_sprite(ball.nb, ball.x, ball.y);

        player.vy = 0;
        opponent.vy = 0;
        delay(40);
    }
}

void main() {
    set_bkg_data(0u, 30u, tileset);
    set_sprite_data(0u, 30u, tileset);
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
    while (TRUE) {
        switch (gameState) {
            case GAME:
                game();
                break;
            case MENU:
                menu();
                break;
            case PAUSE:
                pause();
                break;
            default:
                break;
        }
    }
}