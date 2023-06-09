#include "Player.h"
#include <vector>
using namespace std;
#undef main

const int TILESHEET_WIDTH = 4; //frames per row
long int lastAnimChange = 0;
SDL_Texture* texture;
float angle = 0.0f;

Player::Player(SDL_Renderer* sdlRenderer, int _windowWidth, int _windowHeight)
{
	//variables
	renderer = sdlRenderer;
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
}

void Player::init()
{
	//import sprite with image loader
	SDL_Surface* image = IMG_Load("assets/playersheet.png");
	
	if (image == nullptr)
	{
		//if img loader error
		std::cout << "Could not load image!" << std::endl;
		return;
	}

	// Store the width and height of the sprite sheet image
	textureWidth = image->w;
	textureHeight = image->h;

	// Calculate the width and height of each frame in the sprite sheet
	int frameWidth = textureWidth / TILESHEET_WIDTH;
	int frameHeight = textureHeight / TILESHEET_WIDTH;

	// Create the texture from the sprite sheet image
	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == nullptr)
	{
		//if img loader error
		std::cout << "Could not load texture!" << std::endl;
		return;
	}
	SDL_FreeSurface(image);
	//initialize the last animation change time
	lastAnimChange = SDL_GetTicks();
}

//player position
void Player::SetPosition(int x, int y) {
	portion.x = x;
	portion.y = y;
}

void Player::render()
{
	//player dimensions
	float playerWidth = 54 / 2;
	float playerHeight = 133 / 2;
	portion.w = (int)playerWidth;
	portion.h = (int)playerHeight;

	// Calculate the x and y coordinates of the current frame in the sprite sheet
	int frameX = (frameColumn % NUM_COLUMNS) * FRAME_WIDTH;
	int frameY = (frameRow % NUM_ROWS) * FRAME_HEIGHT;

	// Set the source rectangle for the current frame
	SDL_Rect srcRect = { frameX, frameY, FRAME_WIDTH, FRAME_HEIGHT };

	// Render the current frame of the sprite sheet
	SDL_RenderCopyF(renderer, texture, &srcRect, &portion);

	// Render the health bar
	renderHealthBar(renderer, 100, 10);
}


void Player::renderHealthBar(SDL_Renderer* renderer, int x, int y) {
	// Calculate the width of the health bar based on the player's remaining health
	int barWidth = (int)(health * maxBarWidth / maxHealth);

	// Set the color of the health bar
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	// Render the health bar
	SDL_Rect healthBar = { x, y, barWidth, barHeight };
	SDL_RenderFillRect(renderer, &healthBar);
}

//update for movement
void Player::update()
{
	//movement for keys pressed
	if (keyStates[DOWN])
	{
		moveDown();
		frameRow = 0; // Down direction
		frameColumn++;
		if (frameColumn >= NUM_COLUMNS)
		{
			frameColumn = 0; // Reset to first column
		}
		angle = 180.0f;

	}
	if (keyStates[RIGHT])
	{
		moveRight();
		frameRow = 1; // Right direction
		frameColumn++;
		if (frameColumn >= NUM_COLUMNS)
		{
			frameColumn = 0; // Reset to first column
		}
		angle = 90.0f;

	}
	if (keyStates[UP])
	{
		moveUp();
		frameRow = 2; // Up direction
		frameColumn++;
		if (frameColumn >= NUM_COLUMNS)
		{
			frameColumn = 0; // Reset to first column
		}
		angle = 0.0f;

	}
	if (keyStates[LEFT])
	{
		moveLeft();
		frameRow = 3; // Left direction
		frameColumn++;
		if (frameColumn >= NUM_COLUMNS)
		{
			frameColumn = 0; // Reset to first column
		}
		angle = 270.0f;
	}
}

//cleans
void Player::clean()
{
	//will remove player texture when cleaned
	SDL_DestroyTexture(texture);
}