/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h> // IMG_Load
#include <SDL/SDL_mixer.h>
#include <assert.h>

#include <constant.h>
#include <misc.h>

SDL_Surface* window;

void window_create(int width, int height) {
	assert(width > 0 && height > 0);

	SDL_WM_SetCaption(WINDOW_NAME, NULL );
	window = SDL_SetVideoMode(width, height, 0, // If bits-per-pixel is 0, it is treated as the current display bits per pixel.
			SDL_HWSURFACE);

	if (window == NULL ) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void window_display_image(SDL_Surface* sprite, int x, int y) {
	assert(window);
	assert(sprite);
	
	SDL_Rect place;
	place.x = x;
	place.y = y;

	SDL_BlitSurface(sprite, NULL, window, &place);
}

//Création des bruitages (ne marche pas à cause de Mix_LoadWAV)
void window_display_chunk(Mix_Chunk* chunk) {
	assert(chunk);
	Mix_PlayChannel(-1,chunk, 1);	
}

//Création des musiques
void window_display_music(Mix_Music* music) {
	assert(music);
	Mix_PlayMusic(music, 1);
}

void window_clear() {
	assert(window);
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
}

void window_refresh() {
	assert(window);
	SDL_Flip(window);
}
