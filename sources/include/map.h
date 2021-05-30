/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70,	   		// 	0111 0000
	CELL_EXPLOSION=0x80,	// 	1000 0000
	CELL_BONUS_BOMB_RANGE_DEC=0xF0,
	CELL_BONUS_BOMB_RANGE_INC=0xA0,
	CELL_BONUS_BOMB_NB_DEC=0xB0,
	CELL_BONUS_BOMB_NB_INC=0xC0,
	CELL_BONUS_MONSTER=0xD0,
	CELL_BONUS_LIFE=0xE0,
	CELL_BOMB_CONTROL=0x90

};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=0x01,
	BONUS_BOMB_RANGE_INC=0x02,
	BONUS_BOMB_NB_DEC=0x03,
	BONUS_BOMB_NB_INC=0x04,
	BONUS_MONSTER=0x05,
	BONUS_LIFE=0x06
};

enum scenery_type {
	SCENERY_STONE = 1,    // 0001 
	SCENERY_TREE  = 2,    // 0010 
	SCENERY_RONFLEX  = 3,    // 0011 
	SCENERY_PRINCESS = 4,  // 0100 
	SCENERY_POKEFLUTE = 5,  // 0101
	SCENERY_EAU=6, // 0110
	SCENERY_LAVE=7,
	SCENERY_ARBRE_NEIGE=8,
	SCENERY_ROCHER_NEIGE=9
	
	
};

enum compose_type {
	CELL_TREE       = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE      = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS   = CELL_SCENERY | SCENERY_PRINCESS,
	CELL_RONFLEX    = CELL_SCENERY | SCENERY_RONFLEX,
	CELL_POKEFLUTE  = CELL_SCENERY | SCENERY_POKEFLUTE,
	CELL_EAU= CELL_SCENERY | SCENERY_EAU,
	CELL_LAVE  = CELL_SCENERY | SCENERY_LAVE,
	CELL_ARBRE_NEIGE = CELL_SCENERY | SCENERY_ARBRE_NEIGE,
	CELL_ROCHER_NEIGE = CELL_SCENERY | SCENERY_ROCHER_NEIGE,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,
    CELL_BOX_MONSTER  = CELL_BOX | BONUS_MONSTER,
};

struct map;

//état de la porte
int is_open(short level);

//On ouvre les portes de cette map
void set_door_opened(struct map* map, short level);

//on enlève Ronflex de la map
void set_ronflex_out(struct map* map);

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);

// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Return the type of a scenery
enum scenery_type map_get_scenery_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);
int state_tab_door(int i);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static(short level);

// Display the map on the screen
void map_display(struct map* map,int range,short velocity);

// Display the background
void display_background(int state);

// Display the foreground
void display_foreground(int state);

int binentier(struct map*map, unsigned char type);

//On clear le tableau de monstre avant de recharger une partie
void clearmonster(struct map * map,short level);

//Le monstre se déplace, déplaçons aussi sa structure
void change_monster_list(int x, int y, int new_x, int new_y, short level);

#endif /* MAP_H_ */
