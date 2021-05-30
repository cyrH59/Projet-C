/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>



int main(int argc, char *argv[]) {

   SDL_Event event;
   SDL_WM_SetCaption("SDL_Mixer", NULL);
   if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
      printf("%s", Mix_GetError());
   }
   Mix_Music *musique; //Création du pointeur de type Mix_Music
   musique = Mix_LoadMUS("music/start.mp3"); //Chargement de la musique
   Mix_PlayMusic(musique, -1); //Jouer infiniment la musique

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	struct game* game = game_new();

	window_create(SIZE_BLOC * STATIC_MAP_WIDTH,
	SIZE_BLOC * STATIC_MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	// game loop
	// static time rate implementation
	int done = 0;
	while (!done) {
		timer = SDL_GetTicks();

		done = game_update(game);
		game_display(game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	game_free(game);
 	Mix_CloseAudio();    
	Mix_FreeMusic(musique); //Libération de la musique	
	SDL_Quit();

	return EXIT_SUCCESS;
}


