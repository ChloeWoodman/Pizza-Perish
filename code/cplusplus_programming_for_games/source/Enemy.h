#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "BulletManager.h"
#include "Player.h"
#include <cmath>

struct enemy
{
	float x, y, w, h;
	int health = 100;
	bool dead = false;
	//SDL_Rect enemyRect;
	std::vector<SDL_Rect> enemyRects;
};


class Enemy
{
	friend class GameLoop;
public:
	Enemy(SDL_Renderer* renderer, int window_Width, int window_Height);
	~Enemy() {
		SDL_DestroyTexture(texture1); SDL_DestroyTexture(texture2);
	}
	void init();
	void render();
	void update(Player& player);
	void deleteDeadEnemies();
	void reset();

private:
	float enemy2Width = 40;
	float enemy2Height = 133 / 2;
	float enemyWidth = 54 / 2;
	float enemyHeight = 133 / 2;
	SDL_Renderer* renderer;
	SDL_Texture* texture1;
	SDL_Texture* texture2;
	std::vector<enemy> enemies;
	int TOTAL_ENEMY_NUM = 100;
	const int RESET_THRESHOLD = 70;
	int windowWidth, windowHeight;
};

