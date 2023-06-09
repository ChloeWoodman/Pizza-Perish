#pragma once

#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <chrono>

#include "Player.h"
#include "FontRenderer.h"
#include "TiledMap.h"
#include "BulletManager.h"
#include "Enemy.h"


#define PI 3.14159265 //converts to radians as sin and cos functions

//screen proportions
const int screenWidth = 800;
const int screenHeight = 600;



class GameLoop
{
	int score;
	char score_text[16];
	char highscore_text[34];
	char time_text[16];
	int countdown = 3000;  //duration in seconds
	const int time_down = 1; //amount being taken from time per sec
	const int barHeight = 10;
	int damage = 10;

public:
	int init();
	bool keepAlive();
	void update();
	void render();
	void clean();

	bool getKeyDown(int keyCode) {
		if (keyCode > 0 && keyCode < 512) {
			return this->keyDown[keyCode];
		}
		return false;
	}


private:
	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::unique_ptr<FontRenderer> fontRenderer;
	std::unique_ptr<TiledMap> tiledMap;
	BulletManager* bm;
	bool keyDown[512];
	Player* player = nullptr;
	Enemy* enemy = nullptr;
};

