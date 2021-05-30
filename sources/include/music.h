/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>

// load game's musics
void	   sound_load();

//Chunks
Mix_Chunk* music_get_chunk();

//Musics
Mix_Music* music_get_start();
Mix_Music* music_get_boss();
Mix_Music* music_get_frimapic();
Mix_Music* music_get_foret();
Mix_Music* music_get_bourg();
Mix_Music* music_get_lvl2();
Mix_Music* music_get_festin();
Mix_Music* music_get_galactic();
Mix_Music* music_get_victory();

#endif /* _MUSIC_H_ */