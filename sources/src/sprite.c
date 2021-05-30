/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/

// https://pub.phyks.me/sdz/sdz/jouer-du-son-avec-sdl-mixer.html

#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>

// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR_OPENED	"sprite/door_opened.png"
#define MAP_DOOR_CLOSED	"sprite/door_closed.png"

#define PAUSE		    "sprite/pause.png"

//Sprites of monsters

#define MONSTER_LEFT     "sprite/monster_left.png"
#define MONSTER_UP       "sprite/monster_up.png"
#define MONSTER_RIGHT    "sprite/monster_right.png"
#define MONSTER_DOWN     "sprite/monster_down.png"



// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"
#define MAP_PRINCESS    "sprite/bomberwoman.png" //ajout
#define MAP_RONFLEX     "sprite/ronflex.png" //ajout
#define MAP_POKEFLUTE   "sprite/pokeflute.png" //ajout
#define MAP_BACKGROUND_ROCHE "sprite/background.png" //ajout
#define MAP_BACKGROUND_NEIGE "sprite/fond_neige.png"
#define MAP_BACKGROUND_VOLCAN "sprite/fond_lave.png"
#define MAP_BACKGROUND_HERBE  "sprite/fond_herbe.png"
#define MAP_ARBRE_NEIGE  "sprite/arbre_neige.png"
#define MAP_CASE_LAVE "sprite/volcan_case3.png"
#define MAP_CASE_EAU  "sprite/eau.png"
#define MAP_ROCHER_NEIGE  "sprite/rocherneige.png"
//#define HELP "sprite/help.png"


// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb1.png"
#define BANNER_RANGE	"sprite/banner_range.png"
//#define BANNER_LEVEL "sprite/banner_level.png"
#define DIGIT_0			"sprite/banner_0.jpg"
#define DIGIT_1			"sprite/banner_1.jpg"
#define DIGIT_2			"sprite/banner_2.jpg"
#define DIGIT_3			"sprite/banner_3.jpg"
#define DIGIT_4			"sprite/banner_4.jpg"
#define DIGIT_5			"sprite/banner_5.jpg"
#define DIGIT_6			"sprite/banner_6.jpg"
#define DIGIT_7			"sprite/banner_7.jpg"
#define DIGIT_8			"sprite/banner_8.jpg"
#define DIGIT_9			"sprite/banner_9.jpg"

// badge

#define BADGE_1 "sprite/badge1.png"
#define BADGE_2 "sprite/badge2.png"
#define BADGE_3 "sprite/badge3.png"
#define BADGE_4 "sprite/badge4.png"
#define BADGE_5 "sprite/badge5.png"
#define BADGE_6 "sprite/badge6.png"
#define BADGE_7 "sprite/badge7.png"
#define BADGE_8 "sprite/badge8.png"

// Sprites of Bombs
#define BOMB_TTL1       "sprite/bomb1.png"
#define BOMB_TTL2       "sprite/bomb2.png"
#define BOMB_TTL3       "sprite/bomb3.png"
#define BOMB_TTL4       "sprite/bomb4.png"
#define EXPLOSION       "sprite/explosion.png" //ajout
#define BOMB_CONTROL    "sprite/bomb_control.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

//Sprites of foregrounds 

#define DEAD_PRINCESS    "sprite/foreground2.png"
#define MODE_EMPLOI "sprite/Menu_aide.png"
#define MENU1 "sprite/menu1.png"
#define MENU2 "sprite/menu2.png"
#define GAME_OVER "sprite/gameover.png"
#define CHEATMENU "sprite/cheatmenu.png"
#define VICTORY "sprite/menuvictoire.png"
#define VICTORY2 "sprite/menuvictoires.png"

// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;
SDL_Surface* badge[8];
//SDL_Surface* help;

//SDL_Surface* banner_level;
// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door_opened;
SDL_Surface* door_closed;
SDL_Surface* stone;
SDL_Surface* ronflex;
SDL_Surface* map_arbre_neige;
SDL_Surface* rocherneige;
SDL_Surface* casevolcan;
SDL_Surface* caseeau;
SDL_Surface* pokeflute;
SDL_Surface* princess;
SDL_Surface* backgroundroche;
SDL_Surface* backgroundneige;
SDL_Surface* backgroundvolcan;
SDL_Surface* backgroundherbe;
SDL_Surface* tree;
SDL_Surface* bomb1;
SDL_Surface* bomb2;
SDL_Surface* bomb3;
SDL_Surface* bomb4;
SDL_Surface* bombcontrol;
SDL_Surface* explosion;
//Pause

SDL_Surface* pause;

//foreground
SDL_Surface* deadprincess;
SDL_Surface* menuaide;
SDL_Surface* menu1;
SDL_Surface* menu2;
SDL_Surface* game_over;
SDL_Surface* cheatmenu;
SDL_Surface* victory;
SDL_Surface* victory2;

// bonus
#define NB_BONUS 4
SDL_Surface* bonus[NB_BONUS + 1];

// player
SDL_Surface* player_img[4];

//monster

SDL_Surface* monster_img[4];

static void banner_load() {
	// numbers imgs
	numbers[0] = image_load(DIGIT_0);
	numbers[1] = image_load(DIGIT_1);
	numbers[2] = image_load(DIGIT_2);
	numbers[3] = image_load(DIGIT_3);
	numbers[4] = image_load(DIGIT_4);
	numbers[5] = image_load(DIGIT_5);
	numbers[6] = image_load(DIGIT_6);
	numbers[7] = image_load(DIGIT_7);
	numbers[8] = image_load(DIGIT_8);
	numbers[9] = image_load(DIGIT_9);

	// other banner sprites
	banner_life = image_load(BANNER_LIFE);
	banner_bomb = image_load(BANNER_BOMB);
	banner_range = image_load(BANNER_RANGE);
	banner_line = image_load(BANNER_LINE);
	//banner_level=image_load(BANNER_LEVEL)

	//badge

	badge[0] = image_load(BADGE_1);
	badge[1] = image_load(BADGE_2);
	badge[2] = image_load(BADGE_3);
	badge[3] = image_load(BADGE_4);
	badge[4] = image_load(BADGE_5);
	badge[5] = image_load(BADGE_6);
	badge[6] = image_load(BADGE_7);
	badge[7] = image_load(BADGE_8);



}

static void level_load(){

}

static void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_life);

	//badge 

	for (int i = 0; i < 8; i++) {
		SDL_FreeSurface(badge[i]);
	}
}

static void map_load() {
	// Sprite loading
	tree = image_load(MAP_TREE);
	box = image_load(MAP_CASE);
	key = image_load(MAP_KEY);
	stone = image_load(MAP_STONE);
	ronflex = image_load(MAP_RONFLEX);
	pokeflute = image_load(MAP_POKEFLUTE);
	princess = image_load(MAP_PRINCESS);
	backgroundroche = image_load(MAP_BACKGROUND_ROCHE);
	backgroundneige = image_load(MAP_BACKGROUND_NEIGE);
	backgroundherbe = image_load(MAP_BACKGROUND_HERBE);
	backgroundvolcan = image_load(MAP_BACKGROUND_VOLCAN);
	door_opened = image_load(MAP_DOOR_OPENED);
	door_closed = image_load(MAP_DOOR_CLOSED);
	pause = image_load(PAUSE);
	bomb1 = image_load(BOMB_TTL1);
	bomb2 = image_load(BOMB_TTL2);
	bomb3= image_load(BOMB_TTL3);
	bomb4 = image_load(BOMB_TTL4);
	bombcontrol= image_load(BOMB_CONTROL);
	explosion = image_load(EXPLOSION);
	map_arbre_neige=image_load(MAP_ARBRE_NEIGE);
	casevolcan=image_load(MAP_CASE_LAVE);
	caseeau=image_load(MAP_CASE_EAU);
	rocherneige=image_load(MAP_ROCHER_NEIGE);
}

static void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(ronflex);
	SDL_FreeSurface(pokeflute);
	SDL_FreeSurface(princess);
	SDL_FreeSurface(backgroundroche);
	SDL_FreeSurface(backgroundneige);
	SDL_FreeSurface(backgroundvolcan);
	SDL_FreeSurface(backgroundherbe);
	SDL_FreeSurface(door_opened);
	SDL_FreeSurface(door_closed);
	SDL_FreeSurface(pause);
	SDL_FreeSurface(map_arbre_neige);
	SDL_FreeSurface(casevolcan);
	SDL_FreeSurface(caseeau);
	SDL_FreeSurface(rocherneige);

}

//unload des bombes
static void bomb_unload() {
SDL_FreeSurface(bomb1);
SDL_FreeSurface(bomb2);
SDL_FreeSurface(bomb3);
SDL_FreeSurface(bomb4);
SDL_FreeSurface(explosion);
SDL_FreeSurface(bombcontrol);
}

static void bonus_load() {
	bonus[0] = NULL;
	bonus[BONUS_BOMB_RANGE_INC] = image_load(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = image_load(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = image_load(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = image_load(IMG_BONUS_BOMB_NB_DEC);
}

static void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		if(bonus[i])
			SDL_FreeSurface(bonus[i]);
}

//Load des monstres
static void monster_load() {
	monster_img[WEST] = image_load(MONSTER_LEFT);
	monster_img[EAST] = image_load(MONSTER_RIGHT);
	monster_img[NORTH] = image_load(MONSTER_UP);
	monster_img[SOUTH] = image_load(MONSTER_DOWN);
}

static void player_load() {
	player_img[WEST] = image_load(PLAYER_LEFT);
	player_img[EAST] = image_load(PLAYER_RIGHT);
	player_img[NORTH] = image_load(PLAYER_UP);
	player_img[SOUTH] = image_load(PLAYER_DOWN);
}

static void foreground_load() {
	deadprincess = image_load(DEAD_PRINCESS);
	menuaide= image_load(MODE_EMPLOI);
	menu1= image_load(MENU1);
	menu2= image_load(MENU2);
	game_over=image_load(GAME_OVER);
	cheatmenu=image_load(CHEATMENU);
	victory=image_load(VICTORY);
	victory2=image_load(VICTORY2);
}

static void foreground_unload() {
	SDL_FreeSurface(deadprincess);
	SDL_FreeSurface(menuaide);
	SDL_FreeSurface(menu1);
	SDL_FreeSurface(menu2);
	SDL_FreeSurface(game_over);
	SDL_FreeSurface(cheatmenu);
	SDL_FreeSurface(victory);
	SDL_FreeSurface(victory2);

}

static void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

//Unload des monstres
static void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monster_img[i]);
}

void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	monster_load();
	foreground_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	monster_unload();
	bomb_unload();
	foreground_unload();
}

SDL_Surface* sprite_get_victory(){
	assert(victory);
	return victory ;
}

SDL_Surface* sprite_get_victory2(){
	assert(victory2);
	return victory2 ;
}

SDL_Surface* sprite_get_cheatmenu(){
	assert(cheatmenu);
	return cheatmenu ;
}

SDL_Surface* sprite_get_deadprincess(){
	assert(deadprincess);
	return deadprincess ;
}

SDL_Surface* sprite_get_menuaide(){
	assert(menuaide);
	return menuaide ;
}

SDL_Surface* sprite_get_gameover(){
	assert(game_over);
	return game_over ;
}


SDL_Surface* sprite_get_menu1(){
	assert(menu1);
	return menu1 ;
}

SDL_Surface* sprite_get_menu2(){
	assert(menu2);
	return menu2 ;
}

SDL_Surface* sprite_get_pause(){
	assert(pause);
	return pause ;
}

/*SDL_Surface* sprite_get_help(){
	assert(help);
	return help;
}*/

SDL_Surface* sprite_get_backgroundroche() {
	assert(backgroundroche);
	return backgroundroche;
}
SDL_Surface* sprite_get_backgroundvolcan() {
	assert(backgroundvolcan);
	return backgroundvolcan;
}
SDL_Surface* sprite_get_backgroundherbe() {
	assert(backgroundherbe);
	return backgroundherbe;
}
SDL_Surface* sprite_get_backgroundneige() {
	assert(backgroundneige);
	return backgroundneige;
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number <= 9);
	return numbers[number];
}

SDL_Surface* sprite_get_badge(short level) {
	assert(level >= 0 && level <= 9);
	return badge[level];
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction];
}

//fonction de recupération de l'image des monstres
SDL_Surface* sprite_get_monster(enum direction direction) {
	assert(monster_img[direction]);
	return monster_img[direction];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

/*SDL_Surface* sprite_get_banner_level() {
	assert(banner_level);
	return banner_level;
}*/

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}
//Sprite de Ronflex
SDL_Surface* sprite_get_ronflex() {
	assert(ronflex);
	return ronflex;
}

//Sprite de la pokeflûte
SDL_Surface* sprite_get_pokeflute() {
	assert(pokeflute);
	return pokeflute;
}

//Sprite de la princesse
SDL_Surface* sprite_get_princess() {
	assert(princess);
	return princess;
}

SDL_Surface* sprite_get_door_opened() {
	assert(door_opened);
	return door_opened;
}

SDL_Surface* sprite_get_door_closed() {
	assert(door_closed);
	return door_closed;
}
SDL_Surface* sprite_get_bomb1() {
	assert(bomb1);
	return bomb1;
}
SDL_Surface* sprite_get_bomb2() {
	assert(bomb2);
	return bomb2;
}
SDL_Surface* sprite_get_bomb3() {
	assert(bomb3);
	return bomb3;
}
SDL_Surface* sprite_get_bomb4() {
	assert(bomb4);
	return bomb4;
}
SDL_Surface* sprite_get_explosion() {
	assert(explosion);
	return explosion;
}
SDL_Surface* sprite_get_bombcontrol() {
	assert(bombcontrol);
	return bombcontrol;
}

SDL_Surface* sprite_get_caselave() {
	assert(casevolcan);
	return casevolcan;
}

SDL_Surface* sprite_get_caseeau() {
	assert(caseeau);
	return caseeau;
}

SDL_Surface* sprite_get_arbreneige() {
	assert(map_arbre_neige);
	return map_arbre_neige;
}

SDL_Surface* sprite_get_rocherneige() {
	assert(rocherneige);
	return rocherneige;
}

