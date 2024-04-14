#ifndef DEFS_H
#define DEFS_H

#define FPS 60
#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720

enum {
  CH_ANY = -1,
	CH_PLAYER,
	CH_ENEMY,
	CH_COUNT,
};

enum {
	SND_PLAYER_FIRE,
	SND_PLAYER_DIE,
	SND_PLAYER_HIT,
	SND_COUNT,
};

#define MAX_KEYBOARD_KEYS 350

#define PLAYER_START_X 100
#define PLAYER_START_Y 100
#define PLAYER_SPEED 6
#define PLAYER_BULLET_SPEED 16
#define PLAYER_RELOAD_RATE 8
#define PLAYER_SPEEDUP_RATE 1.8
#define PLAYER_HEALTH 10

#define ENEMY_DY_MAX 2
#define ENEMY_DY_FACTOR 50.0
#define ENEMY_BASE_SPAWN_RATE 14
#define ENEMY_RAND_SPAWN_RATE 20
#define ENEMY_DX_MIN 5
#define ENEMY_DX_MAX 12
#define ENEMY_STARTING_HEALTH 2
#define ENEMY_BULLET_SPEED 10

#define SIDE_PLAYER 0
#define SIDE_ENEMY 1

#define MAX_STARS 500
#define BACKGROUND_SPEED 300

#define FONT_SIZE 12

#endif

