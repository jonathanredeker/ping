// Native
#include <gb/bgb_emu.h>
#include <gb/gb.h>
#include <stdio.h>
#include <types.h>
// Local
#include "include/ball.h"
#include "include/box.h"
#include "include/map.c"
#include "include/paddle.h"
#include "include/tileset.c"
#include "include/util.h"

char debugBuffer[100]; // TEMP
 
enum GameState {
    MENU,
    GAME,
    PAUSE,
    WIN,
    LOSE
};

void menu(UBYTE* gameState) {
    UINT8 input = joypad();
    if (input == J_START) {
        *gameState = GAME;
    }
}

void pause(UBYTE* gameState) {
    waitpad(J_START);
    *gameState = GAME;
    delay(100);
}

void game(BOOLEAN* gameInitialized, UBYTE* gameState) {
    Paddle player;
    Paddle opponent;
    Ball ball;
    
    /* Initialize game objects once so as to maintain their states when 
    switching game states */
    if (!*gameInitialized) {
        *gameInitialized = TRUE;

        /* Compound literals require ISO C99 or later, so I need to use an 
        initialization function to kick-start these game objects */
        init_paddle(
            &player,
            1, 2, 3, // Sprite IDs
            24, 80, // Position
            1, // Velocity Y
            8, 24, // True width and height
            3, 24 // Collision width and height
        );
        init_paddle(
            &opponent,
            4, 5, 6, // Sprite IDs
            144, 80, // Position
            1, // Velocity Y
            8, 24, // True width and height
            3, 24 // Collision width and height
        );
        init_ball(
            &ball,
            0, // Sprite ID
            BALL_DEFAULT_X, BALL_DEFAULT_Y, // Position
            0, 0, // The ball's position in the last frame
            -1, 0, // Velocity vector
            8, 8, // True width and height
            4, 4 // Collision width and height
        );
    
    }

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
        if (input == J_A) {
            *gameState = MENU;
            break;
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
        if (ball.vx == -1 && 
            get_ball_virtual_x(&ball) == player.x + player.w) 
        {
            if (ball.y + (ball.h / 2) > player.y && 
                ball.y + (ball.h / 2) < player.y + player.h) 
            {
                ball.x = ball.last_x;
                ball.x -= ball.vx;
                ball.vx = 1;
            }
        }
        if (ball.vx == 1 && 
            get_ball_virtual_x(&ball) + ball.collision_w == opponent.x) 
        {
            if (
                ball.y + (ball.h / 2) > opponent.y && 
                ball.y + (ball.h / 2) < opponent.y + opponent.h) 
            {
                ball.x = ball.last_x;
                ball.x -= ball.vx;
                ball.vx = -1;
            }
        }

        // Bounds and ball collision detection
        if (get_ball_virtual_y(&ball) <= bounds.y || 
            get_ball_virtual_y(&ball) + ball.collision_h >= bounds.y + bounds.h) 
        {
            ball.vy *= -1;
            // Undo last movement
            ball.y = ball.last_y;
            ball.y -= ball.vy;
        }
        if (get_ball_virtual_x(&ball) <= bounds.x || 
            get_ball_virtual_x(&ball) + ball.collision_w >= bounds.x + bounds.w) 
        {
            ball.vx *= -1;
            // Goal
        }

        ball.last_x = ball.x;
        ball.last_y = ball.y;
        ball.x += ball.vx;
        ball.y += ball.vy;
        move_sprite(ball.nb, ball.x, ball.y);

        player.vy = 0;
        opponent.vy = 0;
        delay(17);
    }
}

void main() {
    UBYTE gameState = MENU;
    BOOLEAN gameInitialized = 0u;
    set_bkg_data(0u, 30u, tileset);
    set_sprite_data(0u, 30u, tileset);
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
    while (TRUE) {
        switch (gameState) {
            case GAME:
                game(&gameInitialized, &gameState);
                break;
            case MENU:
                menu(&gameState);
                break;
            case PAUSE:
                pause(&gameState);
                break;
            default:
                break;
        }
    }
}