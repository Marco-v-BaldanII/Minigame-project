#include <math.h>
#include <iostream>
using namespace std;

#include "Game.h"

Game::Game() {}
Game::~Game() {}

bool Game::Init() {
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Allocating space for new Window nad creating with parameters
	MrWindow = SDL_CreateWindow("George", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	//Check if opened correctly
	if (MrWindow == NULL) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	MrRenderer = SDL_CreateRenderer(MrWindow, -1, SDL_RENDERER_ACCELERATED);
	if (MrRenderer == NULL) {
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}

	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;

	//Initializing Player Entity
	Player.Init(WINDOW_WIDTH >> 1, 500, 100, 60, 5);

	/*Cursor:Init();*/

	//Loading + Initializing BG 
	int h;
	SDL_QueryTexture(bgTexture, NULL, NULL, NULL, &h);
	Background.Init(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);

	idx_shot = 0;

	//Initializing godmode to OFF
	god_mode = false;


	return true;

}

bool Game::LoadImages() {

	//if images cannot be initialized, send appropiate error message
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init, failed to initialize required img support: %s\n", IMG_GetError());
		return false;
	}

	bgTexture = SDL_CreateTextureFromSurface(MrRenderer, IMG_Load("vertical.jpg"));
	if (bgTexture == NULL) {
		SDL_Log("CreateTextureFromSurface failed to load BG: %s\n", SDL_GetError());
		return false;
	}

	playerTexture = SDL_CreateTextureFromSurface(MrRenderer, IMG_Load("Fox.png"));
	if (playerTexture == NULL) {
		SDL_Log("CreateTextureFromSurface failed to load player: %s\n", SDL_GetError());
		return false;
	}

	shotTexture = SDL_CreateTextureFromSurface(MrRenderer, IMG_Load("shot_vertical.png"));
	if (shotTexture == NULL) {
		SDL_Log("CreateTextureFromSurface failed to load shot: %s\n", SDL_GetError());
		return false;
	}

	/*cursorTexture = SDL_CreateTextureFromSurface(MrRenderer, IMG_Load("Cross.png"));
	if (cursorTexture == NULL) {
		SDL_Log("CreateTextureFromSurface failed to load cursor: %s\n", SDL_GetError());
		return false;
	}*/

	return true;
}

void Game::Release()
{
	//Release images
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(shotTexture);
	IMG_Quit();

	//Clean up all SDL initialized subsystems
	SDL_Quit();
}

bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
		/*if (event.type == SDL_MOUSEMOTION)*/
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}

bool Game::Update() {

	if (Input() == false) return true;

	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -1;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 1;
	if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT)	fx = -1;
	if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT)	fx = 1;

	SDL_Event event;

	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);
		Shots[idx_shot].Init(x + 20, y, 20, 50, 10);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
		Shots[idx_shot].Init(x + 50, y, 20, 50, 10);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
	}

	//Logic:

	//BG Scroll
	Background.Move(0, 1);
	if (Background.GetY() <= 0)	Background.SetY(WINDOW_HEIGHT);

	//Player update
	Player.Move(fx, fy);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(0, -1);
			if (Shots[i].GetY() > WINDOW_HEIGHT)	Shots[i].ShutDown();
		}
	}
	return false;

	//Cursor Update

}

void Game::Draw()
{
	SDL_Rect rc;

	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(MrRenderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(MrRenderer);

	//God mode uses red wireframe rectangles for physical objects
	if (god_mode) SDL_SetRenderDrawColor(MrRenderer, 192, 0, 0, 255);

	//Draw background
	Background.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(MrRenderer, bgTexture, NULL, &rc);
	rc.y -= rc.h;

	//Draw player
	Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(MrRenderer, playerTexture, NULL, &rc);
	if (god_mode) SDL_RenderDrawRect(MrRenderer, &rc);

	//Draw shots
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(MrRenderer, shotTexture, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(MrRenderer, &rc);
		}
	}

	//Draw Cursor
	Cursor.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(MrRenderer, cursorTexture, NULL, &rc);


	//Update screen
	SDL_RenderPresent(MrRenderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}
