/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <monster.h>
#include <map.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);
void next_level(struct game* game);
void back_level(struct game* game);

//different foreground
void set_foreground_state(int state);
int get_foreground_state();

// update
int game_update(struct game* game);
int game_level(struct game* game);
#endif /* GAME_H_ */
