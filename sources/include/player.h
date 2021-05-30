/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player ;

//état des portes
int state_tab_key(int i);

//état des pokeflutes
int state_tab_pokeflute(int i);

int tab_key[8];
//condition sur la porte
int door_condition(struct player* player,struct map* map,short level);

//condition sur la clé
int key_conditon(struct player* player,struct map* map,short level);

//condition sur la pokéflute
int pokeflute_conditon(struct player* player,struct map* map,short level);

//On récupére la range pour le fichier game.c
int game_get_range(struct player* player);

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb,int range,int life);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Set, Increase, Decrease the range of bomb that player can put
int  player_get_range_bomb(struct player * player);
void player_inc_range_bomb(struct player * player);
void player_dec_range_bomb(struct player * player);

//Le nombre de vie du joueur
int player_get_nb_life(struct player * player);
void player_inc_life(struct player* player);
void player_dec_life(struct player* player);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map, short level);

// Display the player on the screen
void player_display(struct player* player);

//Player lost a life
void player_lost_a_life();
void player_change_bomb(struct player* player,int bomb);
void player_change_range(struct player* player,int range);
void player_change_life(struct player* player,int life);

void change_key(int k1, int k2, int k3, int k4,int k5, int k6,int k7, int k8);
void change_pokeflute(int k1, int k2, int k3, int k4,int k5, int k6,int k7, int k8);

//Player set a bomb
void player_set_a_bomb(struct player* player, struct map* map,int mode);

#endif /* PLAYER_H_ */
