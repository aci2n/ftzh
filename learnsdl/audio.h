#ifndef AUDIO_H
#define AUDIO_H 1

#include "defs.h"
#include <SDL_mixer.h>

void play_music(char const path[static 1]);
void audio_init();

typedef struct SoundStore SoundStore;
struct SoundStore {
  Mix_Chunk *sounds[SND_COUNT];
};

SoundStore *sound_store_new();
SoundStore *sound_store_init(SoundStore *);
void sound_store_destroy(SoundStore *);

#endif
