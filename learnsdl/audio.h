#ifndef AUDIO_H
#define AUDIO_H 1

#include "defs.h"
#include <SDL_mixer.h>

void play_music(char const path[static 1]);
void audio_init();

typedef struct SoundStore SoundStore;
struct SoundStore {
  Mix_Music *music;
  Mix_Chunk *sounds[SND_COUNT];
};

SoundStore *sound_store_new();
SoundStore *sound_store_init(SoundStore *);
void sound_store_destroy(SoundStore *);
void sound_store_play_music(SoundStore *);
void sound_store_play_sound(SoundStore *, size_t, size_t);

#endif
