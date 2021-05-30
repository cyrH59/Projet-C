/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MISC_H_
#define MISC_H_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

// Print an error message on stderr and exit
void error(const char *s, ...);

// Load an image, raise an error in case of failure
SDL_Surface* image_load(const char *filename);

// Load a background, raise an error in case of failure
SDL_Surface* image_load2(const char *filename);

//Load a music
Mix_Music* music_load(const char* filename);

#endif /* MISC_H_ */
