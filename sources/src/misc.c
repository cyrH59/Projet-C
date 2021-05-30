/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h> // IMG_Load
#include <SDL/SDL_mixer.h>

void error(const char *s, ...) {
	va_list ap;
	va_start(ap, s);

	vfprintf(stderr, s, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

SDL_Surface* image_load(const char* filename) {
	SDL_Surface* img = IMG_Load(filename);
	if (!img)
		error("IMG_Load: %s\n", IMG_GetError());

	return img;
}

SDL_Surface* image_load2(const char* filename) {
	SDL_Surface* img = SDL_LoadBMP(filename);
	if (!img)
		error("IMG_Load: %s\n", IMG_GetError());

	return img;
}

// Pour Load les musiques
Mix_Music* music_load(const char* filename) {
	Mix_Music* sound = Mix_LoadMUS(filename);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
      printf("%s", Mix_GetError());
   }
	return sound;
}

