// Native
#include <gb/bgb_emu.h>
#include <gb/gb.h>
#include <stdio.h>
#include <types.h>
// Local
#include "include/ball.h"
#include "include/rect.h"
#include "include/map.c"
#include "include/paddle.h"
#include "include/tileset.c"
#include "include/util.h"

void menu(UBYTE* gameState);
void pause(UBYTE* gameState);
void game(UBYTE* gameState, BOOLEAN* gameInitialized);

void main() {
    UBYTE gameState = MENU;
    BOOLEAN gameInitialized = FALSE;

    set_bkg_data(0u, 30u, tileset);
    set_sprite_data(0u, 30u, tileset);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
    while (TRUE) {
        switch (gameState) {
            case GAME:
                game(&gameState, &gameInitialized);
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

void menu(UBYTE* gameState) {
    UINT8 input = joypad();
    if (input == J_START) {
        *gameState = GAME;
    }
}

void pause(UBYTE* gameState) {
    waitpad(J_START);
    *gameState = GAME;
    delay(100u);
}

void game(UBYTE* gameState, BOOLEAN* gameInitialized) {
    Paddle player;
    Paddle opponent;
    Ball ball;
    Rect bounds;
    
    /* Initialize game objects once so as to maintain their states when 
    switching game states */
    if (!*gameInitialized) {
        *gameInitialized = TRUE;

        /* Compound literals require ISO C99 or later, so I need to use an 
        initialization function to kick-start these game objects */
        init_paddle(
            &player,
            1u, 2u, 3u, // Sprite IDs
            24u, 80u,   // Position
            1,          // Velocity Y
            8u, 24u,    // True width and height
            3u, 24u     // Collision width and height
        );
        init_paddle(
            &opponent,
            4u, 5u, 6u, // Sprite IDs
            144u, 80u,  // Position
            1,          // Velocity Y
            8u, 24u,    // True width and height
            3u, 24u     // Collision width and height
        );
        init_ball(
            &ball,
            0u,                             // Sprite ID
            BALL_DEFAULT_X, BALL_DEFAULT_Y, // Position
            -1, 0,                          // Velocity vector
            8u, 8u,                         // True width and height
            4u, 4u                          // Collision width and height
        );
        init_rect(
            &bounds,
            8u, 24u,    // Position
            144u, 112u  // Dimensions
        );
    }

    // Always setup background
    set_bkg_tiles(0u, 0u, 20u, 18u, map);

    // Setup player paddle sprites
    set_sprite_tile(player.nb_top, 2u);
    set_sprite_tile(player.nb_middle, 3u);
    set_sprite_tile(player.nb_bottom, 4u);
    move_sprite(player.nb_top, player.x, player.y);
    move_sprite(player.nb_middle, player.x, player.y + 8u);
    move_sprite(player.nb_bottom, player.x, player.y + 16u);
    
    // Setup opponent paddle sprites
    set_sprite_tile(opponent.nb_top, 6u);
    set_sprite_tile(opponent.nb_middle, 7u);
    set_sprite_tile(opponent.nb_bottom, 8u);
    move_sprite(opponent.nb_top, opponent.x, opponent.y);
    move_sprite(opponent.nb_middle, opponent.x, opponent.y + 8u);
    move_sprite(opponent.nb_bottom, opponent.x, opponent.y + 16u);

    // Setup ball sprite
    set_sprite_tile(ball.nb, 5u);
    move_sprite(ball.nb, ball.x, ball.y);

    while (TRUE) {
        UINT8 input = joypad();

        // Player movement
        if (input == J_UP && player.y > bounds.y + OFFSET_Y + 4) {
            player.vy -= 2;
        }
        if (input == J_DOWN && player.y < bounds.y + bounds.h + OFFSET_Y - 4) {
            player.vy += 2;
        }
        if (input == J_A) {
            *gameState = MENU;
            break;
        }
        player.y += player.vy;
        move_sprite(player.nb_top, player.x, player.y);
        move_sprite(player.nb_middle, player.x, player.y + 8u);
        move_sprite(player.nb_bottom, player.x, player.y + 16u);

        
        // Opponent AI movement
        if (opponent.y > ball.y && opponent.y > bounds.y + OFFSET_Y + 4) {
            opponent.vy -= 1;
        } else if (opponent.y < ball.y && 
            opponent.y < bounds.y + bounds.h + OFFSET_Y - 4)
        {
            opponent.vy += 1;
        } else {
            opponent.vy = 0;
        }
        opponent.y += opponent.vy;
        move_sprite(opponent.nb_top, opponent.x, opponent.y);
        move_sprite(opponent.nb_middle, opponent.x, opponent.y + 8u);
        move_sprite(opponent.nb_bottom, opponent.x, opponent.y + 16u);
        
        // Paddle and ball collision detection
        if (ball.vx == -1 && 
            get_ball_virtual_x(&ball) == player.x + player.w) 
        {
            if (ball.y + (ball.h / 2u) > player.y && 
                ball.y + (ball.h / 2u) < player.y + player.h) 
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
                ball.y + (ball.h / 2u) > opponent.y && 
                ball.y + (ball.h / 2u) < opponent.y + opponent.h) 
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
        delay(17u);
    }
}