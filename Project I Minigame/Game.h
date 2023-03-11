#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

#define WINDOW_WIDTH	400
#define WINDOW_HEIGHT	800
#define MAX_KEYS		256
#define MAX_SHOTS		32

#include "Entity.h"

class Game 
{
public:
	Game();
	~Game();

	bool Init();
	void Release();
	bool LoadImages();
	bool Input();
	bool Update();
	void Draw();

private:
	SDL_Window* MrWindow;
	SDL_Renderer* MrRenderer;
	SDL_Texture* bgTexture, * playerTexture, * shotTexture, * cursorTexture;

	Entity Player, Shots[MAX_SHOTS], Cursor, Background;

	bool god_mode;

	int idx_shot;

	enum KEY_STATE { KEY_IDLE, KEY_UP, KEY_DOWN, KEY_REPEAT };
	KEY_STATE keys[MAX_KEYS];

};
