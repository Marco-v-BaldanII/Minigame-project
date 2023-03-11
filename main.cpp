#include <math.h>
#include<time.h>
#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include <iostream>
using namespace std;
#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

int main(int argc, char* args[])
{

	int ShotVelocityX = 0;
	int ShotVelocityY = 0;

	//OPTIMIZED CODE

	SDL_Rect MrRect, EnemyRec, Rad, Shot;
	MrRect.x = 350;
	MrRect.y = 300;
	MrRect.h = 60;
	MrRect.w = 100;

	EnemyRec.x = 350;
	EnemyRec.y = 0;
	EnemyRec.w = 70;
	EnemyRec.h = 70;

	Rad.x = 0;
	Rad.y = 0;
	Rad.w = 50;
	Rad.h = 50;

	Shot = { MrRect.x, MrRect.y, 30,50 };


	// Window pointer
	SDL_Window* MrWindow;

	//Allocating space for new Window nad creating with parameters
	MrWindow = SDL_CreateWindow("George", 0, 40, 400, 800, SDL_WINDOW_SHOWN);
	//Check if opened correctly
	if (MrWindow == NULL) { cout << "Window couldn't be opned correctly"; }


	

	SDL_Renderer* MrRenderer = nullptr;
	SDL_Renderer* EnemyRend = nullptr;


	MrRenderer = SDL_CreateRenderer(MrWindow, -1, SDL_RENDERER_ACCELERATED);
	if (MrRenderer == NULL) { cout << "Window couldn't be opned correctly"; }

	// Load textures
	SDL_Surface* MrSurface = IMG_Load("Fox.png");
	SDL_Texture* MrTexture;
	MrTexture = SDL_CreateTextureFromSurface(MrRenderer, MrSurface);
	SDL_FreeSurface(MrSurface);

	SDL_Surface* EnemySurface = IMG_Load("asteroid.png");
	SDL_Texture* EnemyTexture;
	EnemyTexture = SDL_CreateTextureFromSurface(MrRenderer, EnemySurface);
	SDL_FreeSurface(EnemySurface);

	SDL_Surface* CursorSurf = IMG_Load("Cross.png");
	SDL_Texture* CrossTexture;
	CrossTexture = SDL_CreateTextureFromSurface(MrRenderer, CursorSurf);
	SDL_FreeSurface(CursorSurf);

	SDL_Surface* bgSurface = IMG_Load("vertical.jpg");
	SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(MrRenderer, bgSurface);

	SDL_Surface* shotsurf = IMG_Load("shot.png");
	SDL_Texture* shotTexture = SDL_CreateTextureFromSurface(MrRenderer, shotsurf);
	int targetX = 0;
	int targetY = 0;


	//Background
	
	SDL_Rect BackgroundRect, BackgroundRect2;
	BackgroundRect = { 0,0,400,800 };
	BackgroundRect2 = { 0,-799,400,800 };

	bool fired = false;
	int mouseX = 0;
	int mouseY = 0;
	
	Uint32 lastExecutedTime = 0;
	bool CanFire = true;
	Uint32 shotFiredTime = 0;

	

	//Game LOOP
	bool GameisRunning = true;
	while (GameisRunning == true) {

		

		SDL_Event MrEvent;
		while (SDL_PollEvent(&MrEvent)) {
			//handle events

			//If the event type is to close SDL stop running game
			if (MrEvent.type == SDL_QUIT) {
				GameisRunning = false;
			}

			if (MrEvent.type == SDL_MOUSEMOTION) {
				cout << "The mouse is being moved" << endl;
				Rad.x = MrEvent.motion.x;
				Rad.y = MrEvent.motion.y;
				cout << Rad.x << "___" << Rad.y << endl;

			}
			if (MrEvent.type == SDL_MOUSEBUTTONDOWN && CanFire == true) {
				cout << "Mouse pressed" << endl;


				Uint32 currentTime = SDL_GetTicks(); // Get the current time
				
				if (currentTime - lastExecutedTime >= 2000) {

					lastExecutedTime = currentTime;
					CanFire = false;

					fired = true;
					mouseX = MrEvent.button.x;
					mouseY = MrEvent.button.y;
					int playerCenterX = MrRect.x + MrRect.w / 2;
					int playerCenterY = MrRect.y + MrRect.h / 2;
					int distanceX = mouseX - playerCenterX;
					int distanceY = mouseY - playerCenterY;
					double distance = sqrt(distanceX * distanceX + distanceY * distanceY);
					double directionX = distanceX / distance;
					double directionY = distanceY / distance;
					int speed = 10;
					Shot.x = playerCenterX - Shot.w / 2;
					Shot.y = playerCenterY - Shot.h / 2;
					ShotVelocityX = directionX * speed;
					ShotVelocityY = directionY * speed;
				}
				
				
			}
			if (MrEvent.type == SDL_KEYDOWN) {
				//If statement to only allow the ship to move when shots are unfired
				//Erased cuase it wasn't WORKING :(

				//making the cannnon and ship separate would solve the problem 
				//Bc both depend on ship position movement resets the shot position

					if (MrEvent.key.keysym.sym == SDLK_w) {
						cout << "UP" << endl;
						MrRect.y = MrRect.y - 20;
					}
					if (MrEvent.key.keysym.sym == SDLK_a) {
						cout << "Left" << endl;
						MrRect.x = MrRect.x - 20;
					}
					if (MrEvent.key.keysym.sym == SDLK_d) {
						cout << "Right" << endl;
						MrRect.x = MrRect.x + 20;
					}
					if (MrEvent.key.keysym.sym == SDLK_s) {
						cout << "Down" << endl;
						MrRect.y = MrRect.y + 20;
					}
				
				Shot = { MrRect.x, MrRect.y, 30,50 };
			}
			
			

		}


		Uint32 currentTime = SDL_GetTicks();
		if (!CanFire && currentTime - lastExecutedTime >= 2000) {
			CanFire = true;
		}

		//Scrolling
		SDL_Delay(15);
		BackgroundRect.y += 1;
		BackgroundRect2.y += 1;

		if (BackgroundRect.y > 800) { BackgroundRect.y = -800; }
		if (BackgroundRect2.y > 800) { BackgroundRect2.y = -800; }


		//Update shots
		if (fired == true) {
			
			Shot.x += ShotVelocityX;
			Shot.y += ShotVelocityY;
		}


		


		
		SDL_RenderClear(MrRenderer);

		SDL_SetRenderDrawColor(MrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(MrRenderer);

		SDL_SetRenderDrawColor(MrRenderer, 255, 0, 255, SDL_ALPHA_OPAQUE);


		SDL_RenderCopy(MrRenderer, bgTexture, NULL, &BackgroundRect);
		SDL_RenderCopy(MrRenderer, bgTexture, NULL, &BackgroundRect2);




		SDL_RenderCopy(MrRenderer, EnemyTexture, NULL, &EnemyRec);
		SDL_RenderCopy(MrRenderer, CrossTexture, NULL, &Rad);
		SDL_RenderCopy(MrRenderer, shotTexture, NULL, &Shot);

		SDL_RenderCopy(MrRenderer, MrTexture, NULL, &MrRect);

		SDL_RenderPresent(MrRenderer);
	}


	//Free allocated memory fro window
	SDL_DestroyWindow(MrWindow);
	SDL_DestroyTexture(bgTexture);

	//Turn off SDL systems (video, audio etc...)
	SDL_Quit;
	return 0;
};
