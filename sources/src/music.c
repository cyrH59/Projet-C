/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/

#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <assert.h>
#include <misc.h>

SDL_AudioSpec audioBufferSpec;
Uint8 *audioBuffer;
Uint32 audioBufferLen;

//Son de niveau
#define MUSIC1       "music/test.wav"
#define START       "music/start.mp3"
#define BOSS       "music/legend.mp3"
#define FRIMAPIC       "music/frimapic.mp3"
#define FORET       "music/foret.mp3"
#define BOURG       "music/bourg.mp3"
#define LVL2       "music/lvl2.mp3"
#define FESTIN       "music/festin.mp3"
#define GALACTIC       "music/galactic.mp3"
#define VICTORY       "music/victory.mp3"

//We define our pointer to the music

Mix_Chunk *chunk = NULL;

Mix_Music *start = NULL;
Mix_Music *boss = NULL;
Mix_Music *frimapic = NULL;
Mix_Music *foret = NULL;
Mix_Music *bourg = NULL;
Mix_Music *lvl2 = NULL;
Mix_Music *festin = NULL;
Mix_Music *galactic = NULL;
Mix_Music *victory = NULL;


static void level_music_load() {
    chunk = Mix_LoadWAV(MUSIC1);
	start = Mix_LoadMUS(START);
	boss = Mix_LoadMUS(BOSS);
	frimapic = Mix_LoadMUS(FRIMAPIC);
	foret = Mix_LoadMUS(FORET);
	bourg = Mix_LoadMUS(BOURG);
	lvl2 = Mix_LoadMUS(LVL2);
	festin = Mix_LoadMUS(FESTIN);
	galactic = Mix_LoadMUS(GALACTIC);
	victory = Mix_LoadMUS(VICTORY);
}

static void level_music_unload() {
    Mix_FreeChunk(chunk);
	Mix_FreeMusic(start);
	Mix_FreeMusic(boss);
	Mix_FreeMusic(frimapic);
	Mix_FreeMusic(foret);
	Mix_FreeMusic(bourg);
	Mix_FreeMusic(lvl2);
	Mix_FreeMusic(festin);
	Mix_FreeMusic(galactic);
	Mix_FreeMusic(victory);
}

//We load the musics
void sound_load() {
	level_music_load();
}

//We unload the musics
void sound_free() {
	level_music_unload();
}

Mix_Chunk* music_get_chunk(){
	assert(chunk);
	return chunk;
}

Mix_Music* music_get_start(){
	assert(start);
	return start;
}

Mix_Music* music_get_boss(){
	assert(boss);
	return boss;
}

Mix_Music* music_get_frimapic(){
	assert(frimapic);
	return frimapic;
}

Mix_Music* music_get_foret(){
	assert(foret);
	return foret;
}

Mix_Music* music_get_bourg(){
	assert(bourg);
	return bourg;
}

Mix_Music* music_get_lvl2(){
	assert(lvl2);
	return lvl2;
}

Mix_Music* music_get_festin(){
	assert(festin);
	return festin;
}

Mix_Music* music_get_galactic(){
	assert(galactic);
	return galactic;
}

Mix_Music* music_get_victory(){
	assert(victory);
	return victory;
}