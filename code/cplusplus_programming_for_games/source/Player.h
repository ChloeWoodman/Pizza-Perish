#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#define PI 3.14159265 //converts to radians as sin and cos functions

//keystates for movement
enum KeyStates {
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
	KEYSTATES_COUNT
};

const int activeSprite[] = 
{
	0, 1, 2 , 4, 5
};

class Player
{
	friend class GameLoop;
	bool gameOver = false;
	int health = 100;
	int maxBarWidth = 70;
	int maxHealth = 100;
	int barHeight = 20;
	int currentFrame = 0;
	int frameColumn = 0; // Initial value of 0
	int frameRow = 0;
	int textureWidth;
	int textureHeight;

	const int NUM_ROWS = 4;
	const int NUM_COLUMNS = 6;
	const int FRAME_WIDTH = 192 / NUM_COLUMNS;
	const int FRAME_HEIGHT = 256 / NUM_ROWS;
public:
	//renderer for player
	Player(SDL_Renderer* sdlRenderer, int _windowWidth, int _windowHeight);
	virtual ~Player() { clean(); }
	void init();
	void render();
	void clean();
	void update();
	void renderHealthBar(SDL_Renderer* renderer, int x, int y);

	//position of player
	void SetPosition(int x, int y);

	float getAngle() const { return angle; }

	//hp for healh bar
	void setHp(int hp) {
		if (hp < 0)
			hp = 0;
		if (hp > maxHp)
			hp = maxHp;
		this->currentHp = hp;
	}


	int getHp() { return currentHp; }

	//player movement using portion ()
	void moveRight() {
		portion.x += speed;
		if (portion.x + portion.w >= windowWidth) {
			portion.x = windowWidth - portion.w;
		}
	}
	void moveLeft() {
		portion.x -= speed;
		if (portion.x <= 0) {
			portion.x = 0;
		}
	}
	void moveUp() {
		portion.y -= speed;
		if (portion.y <= 0) {
			portion.y = 0;
		}
	}
	void moveDown() {
		portion.y += speed;
		if (portion.y + portion.h >= windowHeight) {
			portion.y = windowHeight - portion.h;
		}
	}
	bool keyStates[KeyStates::KEYSTATES_COUNT];

	float getX() const
	{
		return portion.x;
	}
	float getY() const
	{
		return portion.y;
	}

	int getW() const
	{
		return portion.w;
	}
	int getH() const
	{
		return portion.h;
	}


private:
	float speed = 10.0f;
	int windowWidth = 0;
	int windowHeight = 0;
	SDL_FRect portion;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int pixelWidth, maxHp, currentHp;
	float angle;
	int x;
	int y;
};
