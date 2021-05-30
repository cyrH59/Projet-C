/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <music.h>
#include <window.h>
#include <monster.h>
#include <player.h>
#include <game.h>

int tab_door[8] = {0};
int mode_bomb[8] = {1};

//int range = 20;

struct map {
	int width;
	int height;
	unsigned char* grid;
};
//listes des monstres
struct monster* monster_list[STATIC_MAP_WIDTH*STATIC_MAP_HEIGHT*8] = {0};
//position au temps t des monstres initialisés dans list_monster
//int monster_list_position[STATIC_MAP_WIDTH*STATIC_MAP_HEIGHT*8] = {0};
//liste de compteurs pour les déplacement de monste
int mooving_tab[STATIC_MAP_WIDTH*STATIC_MAP_HEIGHT*8] = {0};
//tableau d'état des bombes
int tab_bomb[STATIC_MAP_WIDTH*STATIC_MAP_HEIGHT] = {0};
//tableau d'état des explosions
int tab_explosion[STATIC_MAP_WIDTH*STATIC_MAP_HEIGHT] = {0};
//tableau d'état des bonus
int tab_bonus[STATIC_MAP_WIDTH*STATIC_MAP_HEIGHT] = {0};
//tableau des possibilités de bonus (random)
unsigned char tab_bonus_choice[6] = {BONUS_BOMB_RANGE_DEC,BONUS_BOMB_RANGE_INC,BONUS_BOMB_NB_DEC,
									BONUS_BOMB_NB_INC,BONUS_LIFE,BONUS_MONSTER};

#define CELL(i,j) ( (i) + (j) * map->width)

struct monster* monster;

int state_tab_door(int i){
	return  tab_door[i];
}

//Utilisé lorsqu'on charge une sauvegarde
void clearmonster(struct map * map,short level){
	for(int i = 0;i<(map->width*map->height);i++){
		monster_list[i + (STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = 0 ;
	}
	for (int i = 0; i < STATIC_MAP_WIDTH; i++) {
	  for (int j = 0; j < STATIC_MAP_HEIGHT; j++) {
		  if (map_get_cell_type(map,i,j) == CELL_MONSTER){
			int position = i + j*STATIC_MAP_WIDTH ;
			monster_list[position+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = monster_init();
			monster_set_position(monster_list[position+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level], i, j);
		  }
	  }
	}
}

//Déplacement dans le tableau du pointeur vers la structure du monstre quand le monstre bouge
void change_monster_list(int x, int y, int new_x, int new_y, short level){
		monster_list[new_x + new_y*STATIC_MAP_WIDTH + level*(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)]= monster_list[x + y*STATIC_MAP_WIDTH + level*(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)];
		monster_list[x + y*STATIC_MAP_WIDTH + level*(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)] = 0;
}

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;
	return map;
}

//création d'une fonction pour imprimer un délai -récup sur le net https://www.geeksforgeeks.org/time-delay-c/
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	//4 cas de out of bounds de la map
	if (x < 0){
		return 0;
	}
	if (y < 0){
		return 0;
	}
	if (x > STATIC_MAP_WIDTH -1){
		return 0;
	}
	if (y > STATIC_MAP_HEIGHT -1){
		return 0;
	}
	return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

//Test pour savoir si on peut passer la porte
int is_open(short level){
	if (tab_door[level]== 1){
		return 1;
	}
	return 0;
}

//Utile pour reconnaitre les types de cases secondaires (princesse, ronflexe, pokeflute...)
enum scenery_type map_get_scenery_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}
enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type, short level){

	// bonus is encoded with the 4 most significant bits
	int x1= x/40 ;
	int y1= y/40 ;
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		map_set_cell_type(map,x1,y1,CELL_BONUS_BOMB_RANGE_INC);
		break;
	case BONUS_BOMB_RANGE_DEC:
		map_set_cell_type(map,x1,y1,CELL_BONUS_BOMB_RANGE_DEC);
		break;
	case BONUS_BOMB_NB_DEC:
		map_set_cell_type(map,x1,y1,CELL_BONUS_BOMB_NB_DEC);
		break;
	case BONUS_BOMB_NB_INC:
		map_set_cell_type(map,x1,y1,CELL_BONUS_BOMB_NB_INC);
		break;
	case BONUS_LIFE:
		map_set_cell_type(map,x1,y1,CELL_BONUS_LIFE);
		break;
	case BONUS_MONSTER:
		map_set_cell_type(map,x1,y1,CELL_MONSTER);
		int position = x1 + y1*STATIC_MAP_WIDTH ;
		monster_list[position+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = monster_init();
		monster_set_position(monster_list[position+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level], x1, y1);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;

	case SCENERY_RONFLEX:
		window_display_image(sprite_get_ronflex(), x, y);
		break;

	case SCENERY_POKEFLUTE:
		window_display_image(sprite_get_pokeflute(), x, y);
		break;

	case SCENERY_PRINCESS:
		window_display_image(sprite_get_princess(), x, y);
		break;
	case SCENERY_EAU:
		window_display_image(sprite_get_caseeau(), x, y);
		break;
	
	case SCENERY_LAVE:
		window_display_image(sprite_get_caselave(), x, y);
		break;
	case SCENERY_ARBRE_NEIGE:
		window_display_image(sprite_get_arbreneige(), x, y);
		break;
	
	case SCENERY_ROCHER_NEIGE:
		window_display_image(sprite_get_rocherneige(), x, y);
		break;
	}
	
	
}


//Utile dans le cas ou la case est un monstre
void display_monster(struct map* map, int x, int  y, unsigned char type, short velocity){
	//création du mouvement aléatoire
	int k = 0;
	int level = velocity-1;
	int random_direction [4]= {NORTH,SOUTH,WEST,EAST};
	while (k < 144){
		if (monster_list[k+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level]!= 0){
			mooving_tab[k+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] += 1 ;
			int r2 = rand() % 4;
			if (mooving_tab[k+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] % (300 / velocity) == 0){
				monster_set_current_way(monster_list[k+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level], random_direction[r2]);
				monster_move(monster_list[k+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level], map,level);
			}
		}
		if (monster_list[k+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level]!= 0){
			monster_display(monster_list[k+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level]);
		}
		k+=1;
	}
}

int condition_bomb(struct map* map, int x, int y,int range, int k){
return ((map_is_inside(map, x, y)) && (map_get_cell_type(map, x, y) != CELL_DOOR) && 
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_STONE)&& 
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_ROCHER_NEIGE) && 
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_TREE) && 
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_RONFLEX) && 
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_POKEFLUTE) && 
			(k<range+1)&& 
			(map_get_cell_type(map, x, y) != CELL_BOMB)&&
			(map_get_cell_type(map, x, y) != CELL_KEY)&&
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_LAVE)&&
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_EAU)&&
			(map_get_cell_type(map, x, y) + map_get_scenery_type(map, x, y) != CELL_ARBRE_NEIGE));
			
}

//Fonction qui s'occupe des bombes jusqu'à leur explosion finale
void display_bomb(struct map* map, int x, int  y, unsigned char type, int range,int level){
	int x1 = x /40 ;
	int y1 = y / 40 ;
	tab_bomb[x1+y1*map->width] +=1 ;
	if (tab_bomb[x1+y1*map->width] > 0 && tab_bomb[x1+y1*map->width] < 30){
		window_display_image(sprite_get_bomb1(), x, y);
	}
	if (tab_bomb[x1+y1*map->width] >= 30 && tab_bomb[x1+y1*map->width] < 60){
		window_display_image(sprite_get_bomb2(), x, y);
	}
	if (tab_bomb[x1+y1*map->width] >= 60 && tab_bomb[x1+y1*map->width] < 90){
		window_display_image(sprite_get_bomb3(), x, y);
	}
	if (tab_bomb[x1+y1*map->width] >= 90 && tab_bomb[x1+y1*map->width] < 120){
		window_display_image(sprite_get_bomb4(), x, y);
	}
	if (tab_bomb[x1+y1*map->width] > 121 && tab_bomb[x1+y1*map->width] < 135){
	 	window_display_image(sprite_get_explosion(), x, y);
		int k=1;
		while (condition_bomb(map, x1, y1-k,range,k)){
			if (map_get_cell_type(map,x1,y1-k) == CELL_BOX){
				tab_bonus[x1+(y1-k)*map->width] = 1 ;
			}
			if (map_get_cell_type(map,x1,y1-k) == CELL_MONSTER){
				monster_list[x1 + (y1-k)*STATIC_MAP_WIDTH +(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = 0;
			}
			if (map_get_cell_type(map, x1, y1-k) + map_get_scenery_type(map, x1, y1-k) == CELL_PRINCESS){
				set_foreground_state(2);
			}
			map_set_cell_type(map,x1,y1-k,CELL_EXPLOSION);
			tab_explosion[x1+(y1-k)*map->width] = 1 ;
			k++;
		}
		k=1;
		while (condition_bomb(map, x1, y1+k,range,k)){
			if (map_get_cell_type(map,x1,y1+k) == CELL_BOX){
				tab_bonus[x1+(y1+k)*map->width] = 1 ;
			}
			if (map_get_cell_type(map,x1,y1+k) == CELL_MONSTER){
				monster_list[x1 + (y1+k)*STATIC_MAP_WIDTH +(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = 0;
			}
			if (map_get_cell_type(map, x1, y1+k) + map_get_scenery_type(map, x1, y1+k) == CELL_PRINCESS){
				set_foreground_state(2);
			}
			map_set_cell_type(map,x1,y1+k,CELL_EXPLOSION);
			tab_explosion[x1+(y1+k)*map->width] = 1 ;
			k++;
		}
		k=1;
		while (condition_bomb(map, x1-k, y1,range,k)){
			if (map_get_cell_type(map,x1-k,y1) == CELL_BOX){
				tab_bonus[(x1-k)+(y1)*map->width] = 1 ;
			}
			if (map_get_cell_type(map,x1-k,y1) == CELL_MONSTER){
				monster_list[x1-k + (y1)*STATIC_MAP_WIDTH +(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = 0;
			}
			if (map_get_cell_type(map, x1-k, y1) + map_get_scenery_type(map, x1-k, y1) == CELL_PRINCESS){
				set_foreground_state(2);
			}
			map_set_cell_type(map,x1-k,y1,CELL_EXPLOSION);
			tab_explosion[(x1-k)+(y1)*map->width] = 1 ;
			k++;
		}
		k=1;
		while (condition_bomb(map, x1+k,y1,range,k)){
			if (map_get_cell_type(map,x1+k,y1) == CELL_BOX){
				tab_bonus[(x1+k)+y1*map->width] = 1 ;
			}
			if (map_get_cell_type(map,x1+k,y1) == CELL_MONSTER){
				monster_list[x1+k + (y1)*STATIC_MAP_WIDTH +(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = 0;
			}
			if (map_get_cell_type(map, x1+k, y1) + map_get_scenery_type(map, x1+k, y1) == CELL_PRINCESS){
				set_foreground_state(2);
			}
			map_set_cell_type(map,x1+k,y1,CELL_EXPLOSION);
			tab_explosion[(x1+k)+(y1)*map->width] = 1 ;
			k++;
		}
		
		tab_bomb[x1+y1*map->width]++ ;
		}
	
	else if (tab_bomb[x1+y1*map->width] >= 136){
		for(int i = 0;i<map->width;i ++){
			for(int j = 0;j<map->height;j++){
				if (tab_explosion[i+j*map->width] == 1) {
					
					map_set_cell_type(map,i,j,CELL_EMPTY);
					tab_explosion[i+j*map->width] = 0;
				}
				
			}
		}
	
		map_set_cell_type(map,x1,y1,CELL_EMPTY);
		for(int i = 0;i<map->width;i ++){
			for(int j = 0;j<map->height;j++){
				if (tab_bonus[i+j*map->width] == 1) {
					map_set_cell_type(map,i,j,CELL_BONUS);
					tab_bonus[i+j*map->width] = 0;
				}
			}
		}
		tab_bomb[x1+y1*map->width]=0;
	}		
}


// Permet d'enlever les ronflex de la map quand on utilise une pokeflute 
void set_ronflex_out(struct map* map){
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
		  if (map_get_cell_type(map, i, j) + map_get_scenery_type(map, i, j) == CELL_RONFLEX){
			map_set_cell_type(map,i,j,CELL_EMPTY);
		  }
	  }
	}
	return;
}

//Ouvre la porte du niveau courant
void set_door_opened(struct map* map,short level){
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
		  if (map_get_cell_type(map,i,j) == CELL_DOOR){
			tab_door[level] = 1;
		  }
	  }
	}
	return;
}

void map_display(struct map* map,int range, short monster_velocity){
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);
	int x, y;
	short level = monster_velocity;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;
		int r = rand()%7;
	    unsigned char type = map->grid[CELL(i,j)];
	    
	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y,tab_bonus_choice[r-1],level);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_DOOR:
	      if (tab_door[monster_velocity] == 1 || j <= 2){
				window_display_image(sprite_get_door_opened(), x, y);
			}
		  	else {
				window_display_image(sprite_get_door_closed(), x, y);
			  }
	      break;
		case CELL_MONSTER:
		  display_monster(map,x,y,type,monster_velocity+1);
	        break;
		case CELL_BOMB:
		  display_bomb(map,x,y,type, range, level);
	        break;
		case CELL_EXPLOSION:
		  window_display_image(sprite_get_explosion(), x, y);
	      break;
		case CELL_BONUS_BOMB_RANGE_INC:
			window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
			break;
		case CELL_BONUS_BOMB_RANGE_DEC:
			window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
			break;
		case CELL_BONUS_BOMB_NB_DEC:
			window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
			break;
		case CELL_BONUS_BOMB_NB_INC:
			window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
			break;
		case CELL_BONUS_LIFE:
			window_display_image(sprite_get_banner_life(BONUS_LIFE), x, y);
			break;
		case CELL_BONUS_MONSTER:
			map_set_cell_type(map,i,j,CELL_MONSTER);
			monster_list[(STATIC_MAP_HEIGHT*i + j)+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = monster_init();
			monster_set_position(monster_list[(STATIC_MAP_HEIGHT*i + j)+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level], i, j);
			break;
		}
	  }
	}
}

//Affiche les sols selon les maps
void display_background(int state){
		if (state == 0) {
		window_display_image(sprite_get_backgroundherbe(), 0, 0);
	}
	else if (state == 1) {
		window_display_image(sprite_get_backgroundroche(), 0, 0);
	}
	else if (state == 2) {
		window_display_image(sprite_get_backgroundneige(), 0, 0);
	}
	else if (state == 3) {
		window_display_image(sprite_get_backgroundherbe(), 0, 0);
	}
	else if (state == 4) {
		window_display_image(sprite_get_backgroundherbe(), 0, 0);
	}
	else if (state == 5) {
		window_display_image(sprite_get_backgroundroche(), 0, 0);
	}
	else if (state == 6) {
		window_display_image(sprite_get_backgroundvolcan(), 0, 0);
	}
	else if (state == 7) {
		window_display_image(sprite_get_backgroundroche(), 0, 0);
	}
}

// Affiche les menus selon la variable état
void display_foreground(int state){
	if (state == 0) {
		return;
	}
	else if (state == 1) {
		window_display_image(sprite_get_pause(), 0, 0);
	}
	else if (state == 2) {
		window_display_image(sprite_get_deadprincess(), 0, 0);
	}
	else if (state == 3) {
		window_display_image(sprite_get_menuaide(), 0, 0);
	}
	else if (state == 4) {
		window_display_image(sprite_get_menu1(), 0, 0);
	}
	else if (state == 5) {
		window_display_image(sprite_get_menu2(), 0, 0);
	}
	else if (state == 6) {
    	window_display_image(sprite_get_gameover(), 0, 0);
    }
	else if (state == 7) {
    	window_display_image(sprite_get_victory2(), 0, 0);
    }
	else if (state == 8) {
    	window_display_image(sprite_get_cheatmenu(), 0, 0);
    }

}

//Définition de la map de base (pas dans un fichier donc)
struct map* map_get_static(short level)
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);
	unsigned char themaps[8][STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
		{CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY,
		CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_BOX, CELL_TREE, CELL_POKEFLUTE, CELL_EMPTY, CELL_TREE, CELL_EMPTY,
		CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_RONFLEX, CELL_STONE, CELL_BOX, CELL_BOX, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_TREE, CELL_EMPTY,
		CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
		CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
		CELL_EMPTY, CELL_DOOR, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE , CELL_STONE, CELL_STONE, CELL_STONE,  CELL_STONE, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_STONE, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_KEY,  CELL_STONE, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE,  CELL_STONE, CELL_EMPTY,
		CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY,
		},
		{CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_BOX,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
	  CELL_MONSTER, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
	  CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_MONSTER, CELL_STONE, CELL_EMPTY,
	  CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_DOOR, CELL_STONE , CELL_EMPTY, CELL_STONE, CELL_EMPTY,  CELL_STONE, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_MONSTER, CELL_STONE, CELL_EMPTY,  CELL_STONE, CELL_EMPTY,  CELL_STONE, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY,  CELL_STONE, CELL_EMPTY,
	  CELL_BOX, CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_TREE, CELL_TREE, CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,
	  CELL_TREE, CELL_TREE, CELL_KEY, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX,  CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		},
		{CELL_DOOR, CELL_ROCHER_NEIGE, CELL_KEY, CELL_ROCHER_NEIGE, CELL_BOX, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ARBRE_NEIGE,
	  CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_RONFLEX, CELL_EMPTY, CELL_ARBRE_NEIGE,
	  CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ARBRE_NEIGE,
	  CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_RONFLEX, CELL_EMPTY, CELL_ARBRE_NEIGE,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_DOOR,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_RONFLEX, CELL_EMPTY, CELL_ARBRE_NEIGE,
	  CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE,  CELL_EMPTY, CELL_ARBRE_NEIGE,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_ROCHER_NEIGE,  CELL_EMPTY, CELL_RONFLEX,  CELL_EMPTY, CELL_EMPTY,
	  CELL_ROCHER_NEIGE, CELL_MONSTER, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_POKEFLUTE, CELL_ROCHER_NEIGE, CELL_EMPTY,  CELL_ROCHER_NEIGE,  CELL_EMPTY, CELL_ROCHER_NEIGE,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_RONFLEX,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ROCHER_NEIGE, CELL_EMPTY, CELL_ROCHER_NEIGE,  CELL_EMPTY, CELL_ROCHER_NEIGE,
	  CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_RONFLEX, CELL_EMPTY, CELL_EMPTY,
		},
		{
		CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE,
		CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EAU, CELL_TREE, CELL_TREE,
		CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_TREE,
		CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_TREE,
		CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_TREE,
		CELL_TREE, CELL_TREE, CELL_TREE, CELL_MONSTER, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_RONFLEX, CELL_TREE, CELL_TREE,
		CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_TREE , CELL_TREE, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_TREE,
		CELL_TREE, CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_TREE, CELL_DOOR,  CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_TREE,
		CELL_TREE, CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EAU,  CELL_EMPTY, CELL_TREE,
		CELL_TREE, CELL_POKEFLUTE, CELL_BOX, CELL_EMPTY, CELL_BOX, CELL_KEY, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_BOX, CELL_TREE,
		CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY,  CELL_TREE, CELL_TREE,
		CELL_TREE,  CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE, CELL_TREE,
		},
		{
   		 CELL_DOOR, CELL_EMPTY, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_STONE,
   		 CELL_EMPTY, CELL_EMPTY, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_STONE,
   		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_STONE,
   		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_STONE,
   		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_STONE,
   		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_STONE,
    		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX , CELL_BOX, CELL_BOX, CELL_BOX,  CELL_BOX, CELL_STONE,
   		 CELL_STONE	, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX,  CELL_BOX, CELL_BOX,  CELL_BOX, CELL_STONE,
    		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX,  CELL_BOX, CELL_STONE,
   		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_KEY,  CELL_BOX, CELL_STONE,
   		 CELL_STONE, CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,
   		 CELL_STONE,  CELL_STONE, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_DOOR,
   		 },

			{CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_BONUS, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
		CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_POKEFLUTE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_RONFLEX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
		CELL_EMPTY,  CELL_EMPTY, CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_KEY,
		},
		{
   	 	CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_POKEFLUTE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_LAVE,
		CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_KEY, CELL_EMPTY, CELL_EMPTY,
		CELL_LAVE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_STONE, CELL_STONE,  CELL_STONE, CELL_LAVE,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_BOX,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_RONFLEX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_BOX, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
		CELL_EMPTY,  CELL_EMPTY, CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		
    	},
		{CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_KEY,
		CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
		CELL_BOX, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
		CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_POKEFLUTE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_STONE, CELL_MONSTER,
		CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_STONE, CELL_MONSTER,
		CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_RONFLEX, CELL_STONE, CELL_MONSTER, CELL_STONE, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE,  CELL_STONE, CELL_EMPTY,
		CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_MONSTER, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
		CELL_BOX,  CELL_STONE, CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_PRINCESS,
		}
		};
	// Initialisation de la map courante
	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themaps[level][i];
	// Initialisation des monstres
	for (int i = 0; i < STATIC_MAP_WIDTH; i++) {
	  for (int j = 0; j < STATIC_MAP_HEIGHT; j++) {
		  if (map_get_cell_type(map,i,j) == CELL_MONSTER){
			int position = i + j*STATIC_MAP_WIDTH ;
			monster_list[position+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level] = monster_init();
			monster_set_position(monster_list[position+(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT)*level], i, j);
		  }
	  }
	}
	return map;
}
