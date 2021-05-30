/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <game.h>

int tab_key[8] = {0};
int tab_pokeflute[8] = {0};
int var_game_over=0;

void change_key(int k1, int k2, int k3, int k4,int k5, int k6,int k7, int k8){
	tab_key[0]=k1;
	tab_key[1]=k2;
	tab_key[2]=k3;
	tab_key[3]=k4;
	tab_key[4]=k5;
	tab_key[5]=k6;
	tab_key[6]=k7;
	tab_key[7]=k8;
	

}



void change_pokeflute(int k1, int k2, int k3, int k4,int k5, int k6,int k7, int k8){
	tab_pokeflute[0]=k1;
	tab_pokeflute[1]=k2;
	tab_pokeflute[2]=k3;
	tab_pokeflute[3]=k4;
	tab_pokeflute[4]=k5;
	tab_pokeflute[5]=k6;
	tab_pokeflute[6]=k7;
	tab_pokeflute[7]=k8;
	

}

struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int range; //rajout
	int life; //rajout
	// int mode; 
};

struct player* player_init(int bombs,int range,int life) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = EAST;
	player->bombs = bombs;
	player->range=range; //rajout
	player->life=life;

	return player;
}


//On récupére la range pour le fichier game.c
int game_get_range(struct player* player) {
	assert(player);
	return player->range;
}

void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}

//Plein de fonction de recuperation ou de modification de paramètres relatif au joueur
int state_tab_key(int i){
	return  tab_key[i];
}

int state_tab_pokeflute(int i){
	return  tab_pokeflute[i];
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

int player_get_range_bomb(struct player* player) {
	assert(player);
	return player->range;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	if (player->bombs < 9){
		player->bombs += 1;
	}
}

void player_change_bomb(struct player* player,int bomb){
assert(player);
if (player->bombs < 9){
		player->bombs = bomb;
	}


}

void player_change_range(struct player* player,int range){
assert(player);
if (player->range < 9){
		player->range = range;
	}


}
void player_change_life(struct player* player,int life){
assert(player);
if (player->life < 9){
		player->life = life;
	}


}
void player_dec_nb_bomb(struct player* player) {
	assert(player);
	if (player->bombs > 1){
		player->bombs --;
	}
}

void player_inc_range_bomb(struct player* player) {
	assert(player);
	if (player->range < 9){
		player->range += 1;
	}
}

void player_dec_range_bomb(struct player* player) {
	assert(player);
	if (player->range > 1){
		player->range -= 1;
	}
}

int player_get_nb_life(struct player * player){
	assert(player);
	return player->life;
}

void player_inc_life(struct player* player) {
	assert(player);
	if (player->life < 9){
		player->life += 1;
	}
}


void player_dec_life(struct player* player) {
	assert(player);
	if (player->life > 1){
		player->life -= 1;
	}
	else if (player->life == 1){
        set_foreground_state(6);
    }
}

//Condition pour utiliser la porte
int door_condition(struct player* player,struct map* map, short level){
	for (int x = player_get_x(player)-1; x < player_get_x(player)+2;x ++){
			for (int y = player_get_y(player)-1; y < player_get_y(player)+2;y ++){
				if (x >= 0 && y >= 0 && x < STATIC_MAP_WIDTH && y < STATIC_MAP_HEIGHT ){
					if (is_open(level)==1 || (y <=2 && map_get_cell_type(map, x, y) == CELL_DOOR)){
						return 1;
					}
				}
			}
		}
	return 0;
}

// Condition d'utilisation de la clé
int key_conditon(struct player* player,struct map* map,short level){
	for (int x = player_get_x(player)-1; x < player_get_x(player)+2;x ++){
		for (int y = player_get_y(player)-1; y < player_get_y(player)+2;y ++){
			if (x >= 0 && y >= 0 && x < STATIC_MAP_WIDTH && y < STATIC_MAP_HEIGHT ){
				if (map_get_cell_type(map, x, y) == CELL_DOOR){
					if (tab_key[level] == 1){
						
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
// Condition d'utilisation de la flute
int pokeflute_conditon(struct player* player,struct map* map,short level){
	if (tab_pokeflute[level] == 1){
		for (int x = player_get_x(player)-1; x < player_get_x(player)+2;x ++){
			for (int y = player_get_y(player)-1; y < player_get_y(player)+2;y ++){
				if (x >= 0 && y >= 0 && x < STATIC_MAP_WIDTH && y < STATIC_MAP_HEIGHT ){
					if (map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) == CELL_RONFLEX){
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

//Tests de déplacement du joueur
static int player_move_aux(struct player* player, struct map* map, int x, int y,int level) {
	if (!map_is_inside(map, x, y)) {
		return 0;
	}
	if (get_foreground_state() == 8) {
		for(int k= 0;k<9;k++){
			player_inc_nb_bomb(player);
			player_inc_range_bomb(player);
			player_inc_life(player);
		}
		return 1;
	}
	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		if (map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) == CELL_PRINCESS){
			set_foreground_state(7);
			return 1;
		}
		if (map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) == CELL_POKEFLUTE){
			tab_pokeflute[level] = 1;
			return 1;
		}
		return 0;
		break;

	case CELL_BOX:
		return 1;
		break;

	case CELL_BONUS:
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;
	case CELL_BOMB_CONTROL:
		return 0;
		break;


	case CELL_DOOR:
		return 0;
		break;
	case CELL_KEY:
		tab_key[level] = 1;
		return 1;
		break;
	case CELL_BONUS_BOMB_RANGE_INC:
		player_inc_range_bomb(player);
		break;
	case CELL_BONUS_BOMB_RANGE_DEC:
		player_dec_range_bomb(player);
		break;
	case CELL_BONUS_BOMB_NB_DEC:
		player_dec_nb_bomb(player);
		break;
	case CELL_BONUS_BOMB_NB_INC:
		player_inc_nb_bomb(player);
		break;
	case CELL_BONUS_LIFE:
		player_inc_life(player);
		break;
	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map, short level) {
	int x = player->x;
	int y = player->y;
	int move = 0;
	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1,level)) {
			//Cas des caisses à bouger
			if (map_get_cell_type(map, x, y-1) == CELL_BOX && get_foreground_state() != 8){
				if (player_move_aux(player, map, x, y - 2,level)) {
					if (map_get_cell_type(map, x, y-2) == CELL_EMPTY){
						if (map_get_cell_type(map, x, y) == CELL_BOMB){
							player->y--;
							map_set_cell_type(map, x, y-1, CELL_EMPTY);
							map_set_cell_type(map, x, y-2, CELL_BOX);
							break;
						}
						player->y--;
						move = 1;
						map_set_cell_type(map, x, y-1, CELL_EMPTY);
						map_set_cell_type(map, x, y-2, CELL_BOX);
						break;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			//Cas ou on vient de poser une bombe
			if (map_get_cell_type(map, x, y) == CELL_BOMB){
				player->y--;
				break;
			}

			
			//autres cas
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1,level)) {
			//Cas des caisses à bouger
			if (map_get_cell_type(map, x, y+1) == CELL_BOX && get_foreground_state() != 8){
				if (player_move_aux(player, map, x, y + 2,level)) {
					if (map_get_cell_type(map, x, y+2) == CELL_EMPTY){
						if (map_get_cell_type(map, x, y) == CELL_BOMB){
							player->y++;
							map_set_cell_type(map, x, y+1, CELL_EMPTY);
							map_set_cell_type(map, x, y+2, CELL_BOX);
							break;
						}
						player->y++;
						move = 1;
						map_set_cell_type(map, x, y+1, CELL_EMPTY);
						map_set_cell_type(map, x, y+2, CELL_BOX);
						break;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			//Cas ou on vient de poser une bombe
			if (map_get_cell_type(map, x, y) == CELL_BOMB){
				player->y++;
				break;
			}
			
			//autres cas
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y,level)) {
			//Cas des caisses à bouger
			if (map_get_cell_type(map, x-1, y) == CELL_BOX && get_foreground_state() != 8){
				if (player_move_aux(player, map, x - 2, y,level)) {
					if (map_get_cell_type(map, x-2, y) == CELL_EMPTY){
						if (map_get_cell_type(map, x, y) == CELL_BOMB){
							player->x--;
							map_set_cell_type(map, x-1, y, CELL_EMPTY);
							map_set_cell_type(map, x-2, y, CELL_BOX);
							break;
						}
						player->x--;
						move = 1;
						map_set_cell_type(map, x-1, y, CELL_EMPTY);
						map_set_cell_type(map, x-2, y, CELL_BOX);
						break;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			//Cas ou on vient de poser une bombe
			if (map_get_cell_type(map, x, y) == CELL_BOMB){
				player->x--;
				break;
			}
			/*if (map_get_cell_type(map, x, y) == CELL_MONSTER){
			player_dec_life(struct player* player)}*/
			//autres cas
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y,level)) {
			//Cas des caisses à bouger
			if (map_get_cell_type(map, x+1, y) == CELL_BOX && get_foreground_state() != 8){
				if (player_move_aux(player, map, x + 2, y,level)) {
					if (map_get_cell_type(map, x+2, y) == CELL_EMPTY){
						if (map_get_cell_type(map, x, y) == CELL_BOMB){
							player->x++;
							map_set_cell_type(map, x+1, y, CELL_EMPTY);
							map_set_cell_type(map, x+2, y, CELL_BOX);
							break;
						}
						player->x++;
						move = 1;
						map_set_cell_type(map, x+1, y, CELL_EMPTY);
						map_set_cell_type(map, x+2, y, CELL_BOX);
						break;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			//Cas ou on vient de poser une bombe
			if (map_get_cell_type(map, x, y) == CELL_BOMB){
				player->x++;
				break;
			}
			/*if (map_get_cell_type(map, x, y) == CELL_MONSTER){
			player_dec_life(struct player* player)}*/
			//autres cas
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

//création de la fonction qui pose des bombes
void player_set_a_bomb(struct player* player, struct map* map,int mode ) {
	int x = player->x;
	int y = player->y;
	if (map_get_cell_type(map, x, y) == CELL_EMPTY && (mode ==0)){
		if (player->bombs > 0){
			map_set_cell_type(map, x, y, CELL_BOMB);
			player->bombs --; 
		}
	}
	if (map_get_cell_type(map, x, y) == CELL_EMPTY && (mode ==1)){
		if (player->bombs > 0){
			map_set_cell_type(map, x, y, CELL_BOMB_CONTROL);
			player->bombs -- ;
		}
	}
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}


