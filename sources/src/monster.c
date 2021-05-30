/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <assert.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

//création d'une structure correspondante au monstre
struct monster {
	int x, y;
	enum direction direction;
};
// renvoie la direction actuelle du monstre
enum direction monster_get_direction(struct monster* monster) {
    return monster->direction;
}

//création de l'élément monstre
struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->direction = SOUTH;

	return monster;
}
//Plusieurs fonctions copiés de player mais ici pour monster

void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
}


void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

//création de la fonction déplacement auxiliaire du monstre

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {
	if (!map_is_inside(map, x, y)){
		return 0;
	}
	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_EMPTY:
		return 1;
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_DOOR:
		return 0;
		break;
	case CELL_KEY:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;
	case CELL_BONUS_BOMB_RANGE_INC:
		return 0;
		break;
	case CELL_BONUS_BOMB_RANGE_DEC:
		return 0;
		break;
	case CELL_BONUS_BOMB_NB_DEC:
		return 0;
		break;
	case CELL_BONUS_BOMB_NB_INC:
		return 0;
		break;
	case CELL_BONUS_LIFE:
		return 0;
		break;
	default:
		break;
	}
	// monster has moved
	return 1;
}

int monster_move(struct monster* monster, struct map* map, short level) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)) {
			//Interdit de s'approcher de la porte
			if (map_is_inside(map,x,y-2)) {
				if ((map_get_cell_type(map,x,y-2)==CELL_DOOR) || (map_get_cell_type(map,x,y-2)==CELL_KEY)){
					break;
				}
			}
			if (map_is_inside(map,x+1,y-2)) {
				if ((map_get_cell_type(map,x+1,y-2)==CELL_DOOR) ||  (map_get_cell_type(map,x+1,y-2)==CELL_KEY)){
					break;
				}
			}
			if (map_is_inside(map,x-1,y-2)) {
				if ((map_get_cell_type(map,x-1,y-2)==CELL_DOOR) || (map_get_cell_type(map,x-1,y-2)==CELL_KEY)){
					break;
				}
			}
			//autres cas
			monster->y--;
			move = 1;
			map_set_cell_type(map, x, y-1, CELL_MONSTER);
			change_monster_list(x,y,x,y-1,level);
			map_set_cell_type(map, x, y, CELL_EMPTY);
		}
		break;
	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			//Interdit de s'approcher de la porte
			if (map_is_inside(map,x,y+2)) {
				if ((map_get_cell_type(map,x,y+2)==CELL_DOOR) || (map_get_cell_type(map,x,y+2)==CELL_KEY)){
					break;
				}
			}
			if (map_is_inside(map,x+1,y+2)) {
				if ((map_get_cell_type(map,x+1,y+2)==CELL_DOOR) || (map_get_cell_type(map,x+1,y+2)==CELL_KEY) ){
					break;
				}
			}
			if (map_is_inside(map,x-1,y+2)) {
				if ((map_get_cell_type(map,x-1,y+2)==CELL_DOOR) || (map_get_cell_type(map,x-1,y+2)==CELL_KEY)) {
					break;
				}
			}
			//autres cas
			monster->y++;
			move = 1;
			map_set_cell_type(map, x, y+1, CELL_MONSTER);
			change_monster_list(x,y,x,y+1,level);
			map_set_cell_type(map, x, y, CELL_EMPTY);
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			//Interdit de s'approcher de la porte
			if (map_is_inside(map,x-2,y)) {
				if ((map_get_cell_type(map,x-2,y)==CELL_DOOR) || (map_get_cell_type(map,x-2,y)==CELL_KEY)){
					break;
				}
			}
			if (map_is_inside(map,x-2,y+1)) {
				if ((map_get_cell_type(map,x-2,y+1)==CELL_DOOR) || (map_get_cell_type(map,x-2,y+1)==CELL_KEY)){
					break;
				}
			}
			if (map_is_inside(map,x-2,y-1)) {
				if ((map_get_cell_type(map,x-2,y-1)==CELL_DOOR) || (map_get_cell_type(map,x-2,y-1)==CELL_KEY)){
					break;
				}
			}
			//autres cas
			monster->x--;
			move = 1;
			map_set_cell_type(map, x-1, y, CELL_MONSTER);
			change_monster_list(x,y,x-1,y,level);
			map_set_cell_type(map, x, y, CELL_EMPTY);
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			//Interdit de s'approcher de la porte
			if (map_is_inside(map,x+2,y)) {
				if ((map_get_cell_type(map,x+2,y)==CELL_DOOR) || (map_get_cell_type(map,x+2,y)==CELL_KEY)){
					break;
				}
			}
			if (map_is_inside(map,x+2,y+1)) {
				if ((map_get_cell_type(map,x+2,y+1)==CELL_DOOR) || (map_get_cell_type(map,x+2,y+1)==CELL_KEY)){
					break;
				}
			}
			if (map_is_inside(map,x+2,y-1)) {
				if ((map_get_cell_type(map,x+2,y-1)==CELL_DOOR) || (map_get_cell_type(map,x+2,y-1)==CELL_KEY)){
					break;
				}
			}
			//autres cas
			monster->x++;
			move = 1;
			map_set_cell_type(map, x+1, y, CELL_MONSTER);
			change_monster_list(x,y,x+1,y,level);
			map_set_cell_type(map, x, y, CELL_EMPTY);
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}
//display d'un monstre
void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}