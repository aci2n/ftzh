#include "audio.h"
#include "SDL_mixer.h"
#include "defs.h"

struct SoundStore {
  Mix_Music *music;
  Mix_Chunk *sounds[SND_COUNT];
};

void audio_init() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    fprintf(stderr, "couldn't initialize mixer\n");
    exit(1);
  }
  Mix_AllocateChannels(CH_COUNT);
}

void sound_store_play_music(SoundStore *store) {
  if (!Mix_PlayingMusic()) {
    Mix_PlayMusic(store->music, -1);
  }
}

void sound_store_play_sound(SoundStore *store, size_t sound, size_t channel) {
  if (channel < CH_COUNT && sound < SND_COUNT &&
      store->sounds[sound]) {
    Mix_PlayChannel(channel, store->sounds[sound], 0);
  }
}

SoundStore *sound_store_new() {
  return sound_store_init(malloc(sizeof(SoundStore)));
}

SoundStore *sound_store_init(SoundStore *store) {
  *store = (SoundStore){
      .music = Mix_LoadMUS("sound/mus.opus"),
      .sounds =
          {
              [SND_PLAYER_FIRE] = Mix_LoadWAV("sound/lasershot.ogg"),
              [SND_PLAYER_DIE] = Mix_LoadWAV("sound/playerdeath.ogg"),
							[SND_PLAYER_HIT] = Mix_LoadWAV("sound/playerhit.wav"),
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
  Mix_HaltMusic();
  Mix_FreeMusic(store->music);
  free(store);
}
