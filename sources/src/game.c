/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <music.h>
#include <save.h>

//Déclaration de quelques variables globales utiles pour la suite
int timer = 0;
int timer1 = 0;
int timer3 = 0;
int mode = 0;
int foreground = 4;
int x = 1;
int i =0;


struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory
	sound_load(); //We load the musics

	struct game* game = malloc(sizeof(*game));
	game->levels = 8;
	game->level = 0;
	game->maps = malloc(sizeof(struct map*)*(game->levels));
	//display_foreground(9);
	
	for (int i=0; i<8;i++){
	game->maps[i] = map_get_static(i);
	}

	game->player = player_init(1,2,3); //init
	// Set default location of the player
 	if (game->level ==0){
	player_set_position(game->player, 0, 0);}
	if (game->level ==1){
	player_set_position(game->player, 1, 0);}
	if (game->level ==2){
	player_set_position(game->player, 0, 1);}
	if (game->level ==3){
	player_set_position(game->player, 1, 1);}
	if (game->level ==4){
	player_set_position(game->player, 1, 0);}
	if (game->level ==5){
	player_set_position(game->player, 1, 0);}
	if (game->level ==6){
	player_set_position(game->player, 1, 0);}
	if (game->level ==7){
	player_set_position(game->player, 1, 0);}
	return game;
}

int game_level(struct game* game ){
	return game->level;
}


void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}
 
 // changement de map
struct map* game_set_map(struct game* game, int level) {
	assert(game);
	return game->maps[level];
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

//On a passé une porte qui nous fait avancer
void next_level(struct game* game) {
	
	if (game->level+1 < game->levels){
		game->level++;
		game_set_map(game,game->level);
		}
if (game->level ==0){
	player_set_position(game->player, 0,0);
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_bourg(), -1,2000 );}
	if (game->level ==1){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_lvl2(), -1,2000 );
	player_set_position(game->player, 1, 0);}
	if (game->level ==2){
	player_set_position(game->player, 0, 1);
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_frimapic(), -1,2000 );}
	if (game->level ==3){
	player_set_position(game->player, 1, 1);
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_foret(), -1,2000 );}
	if (game->level ==4){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_festin(), -1,2000 );
	player_set_position(game->player, 1, 0);}
	if (game->level ==5){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_lvl2(), -1,2000 );
	player_set_position(game->player, 1, 0);}
	if (game->level ==6){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_galactic(), -1,2000 );
	player_set_position(game->player, 1, 0);}
	if (game->level ==7){
	player_set_position(game->player, 1, 0);
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_boss(), -1,2000 );}

}

//On a passé une porte qui nous fait reculer
void back_level(struct game* game) {
	
	if (game->level > 0){
		game->level--;
		game_set_map(game,game->level);
		}
	if (game->level ==0){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_bourg(), -1,2000 );
	player_set_position(game->player, 0,6);}
	if (game->level ==1){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_lvl2(), -1,2000 );
	player_set_position(game->player, 5, 7);}
	if (game->level ==2){
	player_set_position(game->player, 10, 4);
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_frimapic(), -1,2000 );}
	if (game->level ==3){
	player_set_position(game->player, 7, 8);
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_foret(), -1,2000 );}
	if (game->level ==4){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_festin(), -1,2000 );
	player_set_position(game->player, 10, 11);}
	if (game->level ==5){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_lvl2(), -1,2000 );
	player_set_position(game->player, 3, 11);}
	if (game->level ==6){
	Mix_HaltMusic();
	Mix_FadeInMusic( music_get_galactic(), -1,2000 );
	player_set_position(game->player, 1, 11);}
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc +SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 2*white_bloc + 2* SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3*SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_range_bomb(game_get_player(game))), x, y);



    x = 4 * white_bloc + 4* SIZE_BLOC;
	window_display_image(sprite_get_badge(game->level), x, y);

		x = 4*white_bloc +5*SIZE_BLOC;
	window_display_image(sprite_get_number(game->level+1), x, y);
}

//état du menu (0=pas de menu)
void set_foreground_state(int state){
	foreground = state ;
	return;
}

// On récupère l'état du menu
int get_foreground_state(){

	return foreground;
}

void game_display(struct game* game) {
	assert(game);
	window_clear();
	display_background(game->level);
	//Mode sans menu ou mode triche, on peut jouer avec ces états là
	if (foreground == 0 || foreground == 8){
		map_display(game_get_current_map(game),game_get_range(game_get_player(game)),game->level);
		player_display(game->player);
		game_banner_display(game);
	}
	display_foreground(foreground);
	window_refresh();
	//Mort
	if (foreground == 6){
		Mix_HaltMusic();
	}
	//Victoire finale
	if (foreground == 7 && Mix_PlayingMusic() == 0){
		Mix_FadeInMusic( music_get_victory(), 1,2000 );
	}
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int timer2 = SDL_GetTicks();
	//codage de la perte de vie + la seconde d'invincibilité
	if (map_get_cell_type(map,player_get_x(player),player_get_y(player)) == CELL_MONSTER || map_get_cell_type(map,player_get_x(player),player_get_y(player)) == CELL_EXPLOSION){
		if (timer2 > timer +1000){
			player_dec_life(player);
			timer = timer2;
		}
	}
	for (int i = 0; i < STATIC_MAP_WIDTH; i++) {
	  for (int j = 0; j < STATIC_MAP_HEIGHT; j++) {
			if (map_get_cell_type(map,i,j) == CELL_EXPLOSION){
				if (timer2 > timer1 +250){
					player_inc_nb_bomb(player);
					timer1 = timer2;
				}
			}
	  }
	}

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				if (foreground==5){
					foreground = 4;
				}
				else {
					player_set_current_way(player, NORTH);
					player_move(player, map,game->level);
				}
				break;
			case SDLK_DOWN:
				if (foreground==4){
					foreground = 5;
				}
				else {
					player_set_current_way(player, SOUTH);
					player_move(player, map,game->level);
				}
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map,game->level);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map,game->level);
				break;
			case SDLK_z:
				if (foreground==5){
					foreground = 4;
				}
				else {
					player_set_current_way(player, NORTH);
					player_move(player, map,game->level);
				}
				break;
			case SDLK_s:
				if (foreground==4){
					foreground = 5;
				}
				else {
					player_set_current_way(player, SOUTH);
					player_move(player, map,game->level);
				}
				break;
			case SDLK_d:
				player_set_current_way(player, EAST);
				player_move(player, map,game->level);
				break;
			case SDLK_q:
				player_set_current_way(player, WEST);
				player_move(player, map,game->level);
				break;
			//On pose des bombes
			case SDLK_SPACE:
				if (timer2 > timer3 +250){
					player_set_a_bomb(player, map,mode); 
					timer3 = timer2;
				}
				break;
			//Pause
			case SDLK_p:
				if (foreground == 1){
					foreground = 0;
					Mix_ResumeMusic();
				}
				else if (foreground == 0){
					foreground=1;
					Mix_PauseMusic();
				}
			// On utilise la clé
			case SDLK_e:
				if (key_conditon(player,map,game->level) == 1){
					set_door_opened(map,game->level);
				}
				break;
			case SDLK_RETURN:
				if (foreground == 4){
					//on reload la map
					clear();
					foreground = 0;
					Mix_FadeInMusic( music_get_bourg(), -1,2000 );
					printf(" Appuyez sur H, si vous avez besoin d'aide \n");
					break;
				}
				if (foreground == 5){
					//on load la save
					 FILE *fichierp1=NULL;
				     fichierp1 = fopen("src/savep1.txt", "r+");
					/* if (ftell(fichierp1) == 0){
						printf("Vous n'avez pas de partie sauvegardée\n");
						break;
					 }*/
					 while (i<lecturefichier(0)){
						 next_level(game);
						 i++;
					 }
					 i=0;

				 
					player_set_position(game->player, lecturefichier(1),lecturefichier(2));
					player_change_bomb(player,lecturefichier(3));
					player_change_range(player,lecturefichier(4));
					player_change_life(player,lecturefichier(5));
					change_key(lecturefichier(6),lecturefichier(7),lecturefichier(8),lecturefichier(9),lecturefichier(10),lecturefichier(11),lecturefichier(12),lecturefichier(13));
					change_pokeflute(lecturefichier(14),lecturefichier(15),lecturefichier(16),lecturefichier(17),lecturefichier(18),lecturefichier(19),lecturefichier(20),lecturefichier(21));
					
					for (int j=0;j<8;j++){
						load_map(game->maps[j],game,j+1);
						clearmonster(game->maps[j],j);
					}
					
					foreground = 0;
					if (game->level == 0){
					Mix_FadeInMusic( music_get_bourg(), -1,2000 );
					}
					if (game->level == 1){
					Mix_FadeInMusic( music_get_lvl2(), -1,2000 );
					}
					if (game->level == 2){
					Mix_FadeInMusic( music_get_frimapic(), -1,2000 );
					}
					if (game->level == 3){
					Mix_FadeInMusic( music_get_foret(), -1,2000 );
					}
					if (game->level == 4){
					Mix_FadeInMusic( music_get_festin(), -1,2000 );
					}
					if (game->level == 5){
					Mix_FadeInMusic( music_get_lvl2(), -1,2000 );
					}
					if (game->level == 6){
					Mix_FadeInMusic( music_get_galactic(), -1,2000 );
					}
					if (game->level == 7){
					Mix_FadeInMusic( music_get_boss(), -1,2000 );
					}
					printf(" Appuyez sur H, si vous avez besoin d'aide \n");
					
					fclose(fichierp1);
					break;
				}
				if (door_condition(player,map,game->level) == 1){
					if (player_get_y(player) <= 2){
						back_level(game);
					}
					else {
						next_level(game);
					}
				}
				break;
			// On utilise la flute
			case SDLK_f:
				if (pokeflute_conditon(player,map,game->level) == 1){
					set_ronflex_out(map);
				}
				break;
			case SDLK_h:
				if (foreground == 3){
					foreground = 0;
				}
				else if (foreground == 0){
					foreground=3;
				}
				break;
			// Se déplacer en avant en trichant
			case SDLK_n:
				if (foreground == 8){
					next_level(game);
				}
				break;
			// Se déplacer en arrière en trichant
			case SDLK_b:
				if (foreground == 8){
					back_level(game);
				}
				break;
			// Activer le mode de triche
			case SDLK_c:
				if (foreground == 0){
					foreground = 8;
				}
				else if (foreground == 8){
					foreground = 0;
				}
				break;
			case SDLK_a:
				if (foreground == 1 || foreground == 6){
				   clear();
				   fichier_savep1(game, player);
				   for (int k=0;k<8;k++){
				   fichier_savemap(game->maps[k],game,k+1);}
					return 1;
				}
				break;
			default:
				break;
			}
			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {
	if (input_keyboard(game)){
		return 1; // exit game
	}
	return 0;
}
