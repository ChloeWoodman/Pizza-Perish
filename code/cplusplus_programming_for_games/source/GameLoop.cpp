#define SDL_MAIN_HANDLED
#include "GameLoop.h"
#include <SDL.h>


int GameLoop::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "could not intialise SDL2!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}

	//makes SDL window
	window = SDL_CreateWindow(
		"Game Window :D",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screenWidth, screenHeight,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL)
	{
		//if window does not load correctly, print error for debugging
		std::cout << "could not intialise window!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "could not intialise mixer!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}

	// Load the audio file
	Mix_Music* music = Mix_LoadMUS("assets/bg_music.mp3");
	if (!music) {
		std::cout << "could not load audio!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}

	// Play the music
	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);


	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {

		//if renderer does not load correctly, print error for debugging
		std::cout << "could not intialise renderer!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}

	for (int i = 0; i < 256; i++) {
		keyDown[i] = false;
	}
	screenSurface = SDL_GetWindowSurface(window);

	player = new Player(renderer, screenWidth, screenHeight);
	player->init();

	bm = new BulletManager(this->renderer, this->player);
	bm->init();


	//render font
	fontRenderer = std::unique_ptr<FontRenderer>(new FontRenderer(renderer));
	fontRenderer->init();

	//set tile maps
	tiledMap = std::unique_ptr<TiledMap>(new TiledMap(renderer, "assets/tmss.png"));
	tiledMap->init();

	//render enemy
	enemy = new Enemy(renderer, screenWidth, screenHeight);
	enemy->init();

	return 0;
}


void GameLoop::update()
{
	player->update();
	bm->update();
	enemy->update(*player);

	// Decrease the time remaining
	countdown -= time_down;

	int time_remaining = countdown / 100;
	sprintf_s(time_text, "Time: %d", time_remaining);

	// Render the health bar for the player
	player->renderHealthBar(renderer, player->getX(), player->getY() - barHeight - 5);

	//bullet enemy intersect
	for (auto& e : enemy->enemies)
	{
		for (auto& b : bm->bullets)
		{
			
			SDL_Rect bulletRect = { b.x, b.y, 40, 40 };
			SDL_Rect enemyRect = { e.x, e.y, e.w, e.h };
			SDL_Rect playerRect = { (int)player->portion.x, (int)player->portion.y, (int)player->portion.w, (int)player->portion.h };
			SDL_Rect nullRect;


			if (SDL_IntersectRect(&enemyRect, &bulletRect, &nullRect))
			{
				//if bullet intersects with enemy, reduce enemy health
				e.health -= 50;
				//print enemy health
				std::cout << e.health << std::endl;
				//if enemy runs out of health, make enemy death true
				if (e.health <= 0) {
					e.dead = true;
					//add player score
					score += 1;
					//print player score
					std::cout << score << std::endl;
				}
				//changes bullet distance after collision with enemy to be off screen
				b.distance = 801;
			}
		}

		//player intersect with enemies
		SDL_Rect playerRect = { (int)player->portion.x, (int)player->portion.y, (int)player->portion.w, (int)player->portion.h };
		SDL_Rect enemyRect = { e.x, e.y, e.w, e.h };
		SDL_Rect nullRect;
		if (SDL_IntersectRect(&enemyRect, &playerRect, &nullRect))
		{
			player->health -= damage;
			e.dead = true;

		}
	}	

	while (player->health <= 0) {
		// Render "Game Over" text
		fontRenderer->render("Game Over", screenWidth - 200, screenHeight - 400, 1000);
		fontRenderer->render("Press 'r' to restart", screenWidth - 200, screenHeight - 300, 1000);


		// Check for keyboard events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				// If the 'r' key is pressed, restart the game
				if (event.key.keysym.sym == SDLK_r) {
					// Reset the player's health and score
					player->health = 100;
					score = 0;
					countdown = 3000;
					int x = 0;
					int y = 0;
					player->SetPosition(x, y);
					enemy->reset();
					//Clear the keyDown array
					for (int i = 0; i < 256; i++) {
						keyDown[i] = false;
					}
					for (int i = 0; i < KeyStates::KEYSTATES_COUNT; i++) {
						player->keyStates[i] = false;
					}
					//Restart the game loop
					break;
				}
			}
		}
		// Present the updated game state
		SDL_RenderPresent(renderer);
	}

	//win when countdown gets to 0
	while (countdown <= 0) {

		sprintf_s(highscore_text, "You win! Score: %d", score);

		// Render "Game Over" text
		fontRenderer->render(highscore_text, screenWidth - 200, screenHeight - 400, 1000);
		fontRenderer->render("Press 'r' to restart", screenWidth - 200, screenHeight - 300, 1000);

		countdown = 0;

		// Check for keyboard events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				// If the 'r' key is pressed, restart the game
				if (event.key.keysym.sym == SDLK_r) {
					// Reset the player's health and score
					player->health = 100;
					score = 0;
					countdown = 3000;
					int x = 0;
					int y = 0;
					player->SetPosition(x, y);
					//Clear the keyDown array
					for (int i = 0; i < 256; i++) {
						keyDown[i] = false;
					}
					for (int i = 0; i < KeyStates::KEYSTATES_COUNT; i++) {
						player->keyStates[i] = false;
					}
					enemy->reset();
					break;
				}
			}
		}
		// Present the updated game state
		SDL_RenderPresent(renderer);
	}
}

void GameLoop::render()
{
	tiledMap->render();
	bm->draw();
	enemy->render();
	player->render();

	sprintf_s(score_text, "Score: %d", score);


	//word input, posx, posy and size
	fontRenderer->render(time_text, screenWidth - 300, screenHeight - 600, 50);
	fontRenderer->render(score_text, screenWidth - 600, screenHeight - 600, 50);
	SDL_RenderPresent(renderer);
	SDL_Delay(16);
}

bool GameLoop::keepAlive()
{
	SDL_Event userInput;
	//keystates for player movement, key down = true will cause movement, when key is not down movement is false
	while (SDL_PollEvent(&userInput))
	{
		//if input of SDL_QUIT, the game will end
		if (userInput.type == SDL_QUIT)
		{
			return false;
		}
		//user inputs for keydowns, movement, if keydown is true then use associated keystate
		if (userInput.type == SDL_KEYDOWN) {
			switch (userInput.key.keysym.sym) {
			case SDLK_LEFT:
			case SDLK_a:
				player->keyStates[LEFT] = true;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				player->keyStates[RIGHT] = true;
				break;
			case SDLK_UP:
			case SDLK_w:
				player->keyStates[UP] = true;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				player->keyStates[DOWN] = true;
				break;
			}
			if (userInput.key.keysym.scancode < 512)
			{
				keyDown[userInput.key.keysym.scancode] = true;
			}
		}
		//user inputs for keydowns, movement, if keydown is false then stop associated keystate
		else if (userInput.type == SDL_KEYUP) {
			switch (userInput.key.keysym.sym) {
			case SDLK_LEFT:
			case SDLK_a:
				player->keyStates[LEFT] = false;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				player->keyStates[RIGHT] = false;
				break;
			case SDLK_UP:
			case SDLK_w:
				player->keyStates[UP] = false;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				player->keyStates[DOWN] = false;
				break;
			}
			if (userInput.key.keysym.scancode < 512)
			{
				keyDown[userInput.key.keysym.scancode] = false;
			}
		}
	}

	//bullet manager keydown
	bm->processInput(keyDown);
	return true;
}

void GameLoop::clean()
{
	//fontRenderer->clean();
	tiledMap->clean();
	delete player, enemy;
	fontRenderer->clean();
	bm->clean();
	SDL_DestroyWindow(window);
	SDL_Quit();
}