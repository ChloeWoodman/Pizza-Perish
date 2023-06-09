#pragma once
#include <SDL.h>
#include <SDL_image.h>
//vector for managing array
#include <vector> //array which will be used to managed bullets within class
#include <math.h> //random cos and sin functions
#include <algorithm> //used for std:remove, if function allows for easy object removal from vectors with condition
#include <SDL_mixer.h>

#include "Player.h"
#define PI 3.14159265 //converts to radians as sin and cos functions

using namespace std; //defined to prevent use of std: before every object/function which is placed under std namespace

struct Bullet {
	float x, y, distance, angle, spin;
};

static Mix_Chunk* soundEffect1;

//this will create instances of the bullet structure and remove them
//also will allow user input to add new bullet when space key is pressed, load the image used for bullet and delete bullet sprite
//this is good for saving memory, saving bullets here instead if duplication graphic being loaded in memory, more efficient
class BulletManager
{
	friend class GameLoop;
public:
	int channel = -1; // Play on the first free channel

	BulletManager(SDL_Renderer* renderer, Player* player) : renderer(renderer), player(player) {}

	//loads the lazer image which is used to draw bullets; kind of like loading sprites
	void init() {
		//load the texture for the pizza bullet
		SDL_Surface* surface = IMG_Load("assets/pizza_attack.png");
		this->bulletTexture = SDL_CreateTextureFromSurface(this->renderer, surface);
		SDL_FreeSurface(surface);

		// Initialize SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			std::cout << "could not intialise mixer!" << std::endl;
			std::cout << SDL_GetError() << std::endl;
			return;
		}

		Mix_Chunk* soundEffect = Mix_LoadWAV("assets/sound_effect.mp3");
		if (!soundEffect) {
			std::cout << "Could not load sound effect!" << std::endl;
			std::cout << SDL_GetError() << std::endl;
			return;
		}

		soundEffect1 = soundEffect;


		// Set the volume of the sound effect to full
		Mix_Volume(channel, MIX_MAX_VOLUME);
		Mix_PlayChannel(channel, soundEffect, 0);
	}

	//check for space key to be pressed and creates new bullet, also timer so player cannot abuse
	void processInput(bool* keyDown) {
		if (keyDown[SDL_SCANCODE_SPACE]) {
			if (SDL_GetTicks() - lastShot > SHOOT_TIMER_MS) {
				// Play the sound effect
				Mix_PlayChannel(-1, soundEffect1, 0);
				// Calculate the angle at which the bullet was fired
				float angle = player->getAngle();

				// Create a new bullet and store the angle in it
				bullets.push_back(Bullet{ player->getX(), player->getY(), 0.0f, angle });
				lastShot = SDL_GetTicks();
			}
		}
	}
	
	void update()
	{
		for (auto& b : bullets)
		{
			// Update the bullet's position using the stored angle and the BULLET_VELOCITY constant
			b.x += sin(b.angle * PI / 180.0f) * BULLET_VELOCITY;
			b.y -= cos(b.angle * PI / 180.0f) * BULLET_VELOCITY;

			// Increment the bullet's distance traveled
			b.distance += BULLET_VELOCITY;

			b.spin += 1;
		}

		// Remove bullets that have traveled more than 800 pixels
		auto remove = std::remove_if(bullets.begin(), bullets.end(),
			[](const Bullet& b) { return b.distance > 800; });
		bullets.erase(remove, bullets.end());
	}

	void draw()
	{
		SDL_Point center = { player->getW() / 2, player->getH() / 2 };

		for (auto& b : bullets)
		{
			SDL_Rect dest = { b.x, b.y, 40, 40 };
			SDL_RenderCopyEx(renderer, bulletTexture, 0, &dest, b.spin, &center, SDL_FLIP_NONE);
		}
	}

	void clean()
	{
		SDL_DestroyTexture(this->bulletTexture);
		//code profiling update
		Mix_FreeChunk(soundEffect1);
	}

private:
	SDL_Rect pizzaRect;
	SDL_Renderer* renderer;
	SDL_Texture* bulletTexture;
	vector<Bullet> bullets;
	//need a reference to the player to get angle
	Player* player;
	//time limit between shots
	const int SHOOT_TIMER_MS = 300;
	//number of pixels travelled per frame
	const float BULLET_VELOCITY = 2.0f;
	unsigned int lastShot = 0;
};
