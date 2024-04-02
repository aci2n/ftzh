#include "audio.h"
#include "defs.h"
#include <SDL_mixer.h>

void audio_init() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
    fprintf(stderr, "couldn't initialize mixer\n");
    exit(1);
  }
  Mix_AllocateChannels(MAX_SOUND_CHANNELS);
}

void play_music(char const path[static 1]) {
  Mix_Music *music = Mix_LoadMUS(path);
  if (!music) {
    fprintf(stderr, "error loading music %s\n", path);
    return;
  }
  Mix_PlayMusic(music, -1);
}
