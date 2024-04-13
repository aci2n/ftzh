#include "audio.h"
#include "SDL_mixer.h"

void audio_init() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
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
  Mix_Volume(-1, 20);
}

SoundStore *sound_store_new() {
  return sound_store_init(malloc(sizeof(SoundStore)));
}

SoundStore *sound_store_init(SoundStore *store) {
  *store = (SoundStore){
      .sounds =
          {
              [SND_PLAYER_FIRE] = Mix_LoadWAV("sound/lasershot.ogg"),
          },
  };
  return store;
}

void sound_store_destroy(SoundStore *store) {
  for (size_t i = 0; i < SND_COUNT; i++) {
    if (store->sounds[i]) {
      Mix_FreeChunk(store->sounds[i]);
    }
  }
  free(store);
}
