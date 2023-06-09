#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, int window_Width, int window_Height) : renderer(renderer), windowWidth(window_Width), windowHeight(window_Height)
{
}

void Enemy::init()
{
	// Load enemy 1 texture
	SDL_Surface* surface = IMG_Load("assets/enemy_1.png");
	if (surface == NULL)
	{
		std::cout << "no enemy pic" << std::endl;
		return;
	}
	texture1 = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Load enemy 2 texture
	surface = IMG_Load("assets/enemy_2.png");
	if (surface == NULL)
	{
		std::cout << "no enemy2 pic" << std::endl;
		return;
	}
	texture2 = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Generate random positions for enemies
	for (int i = 0; i < TOTAL_ENEMY_NUM; i++)
	{
		float x = rand() % 750;
		float y = rand() % 500;
		int type = rand() % 2;
		if (type == 0)
		{
			enemies.push_back({ x, y, enemyWidth, enemyHeight, 100, false, { { (int)x, (int)y, (int)enemyWidth, (int)enemyHeight } } }); // enemy 1
		}
		else
		{
			enemies.push_back({ x, y, enemy2Width, enemy2Height, 100, false, { { (int)x, (int)y, (int)enemy2Width, (int)enemy2Height } } }); // enemy 2
		}
	}
}

void Enemy::render()
{
	for (const auto& enemy : enemies)
	{
		SDL_Texture* texture;
		if (enemy.w == enemyWidth && enemy.h == enemyHeight) {
			texture = texture1;
		}
		else if (enemy.w == enemy2Width && enemy.h == enemy2Height) {
			texture = texture2;
		}
		else {
			// invalid enemy data
			continue;
		}
		for (const auto& rect : enemy.enemyRects) {
			SDL_RenderCopy(renderer, texture, NULL, &rect);
		}
	}

	deleteDeadEnemies();
}

void Enemy::reset()
{
	enemies.clear(); // Clear the current enemy positions

	TOTAL_ENEMY_NUM = 100;

	// Generate new random positions for enemies
	// Generate random positions for enemies
	for (int i = 0; i < TOTAL_ENEMY_NUM; i++)
	{
		float x = rand() % 750;
		float y = rand() % 500;
		int type = rand() % 2;
		if (type == 0)
		{
			enemies.push_back({ x, y, enemyWidth, enemyHeight, 100, false, { { (int)x, (int)y, (int)enemyWidth, (int)enemyHeight } } }); // enemy 1
		}
		else
		{
			enemies.push_back({ x, y, enemy2Width, enemy2Height, 100, false, { { (int)x, (int)y, (int)enemy2Width, (int)enemy2Height } } }); // enemy 2
		}
	}
}


void Enemy::update(Player& player)
{
	float playerX = player.getX();
	float playerY = player.getY();

	for (int i = 0; i < enemies.size(); i++)
	{
		// Calculate the distance between the enemy and player
		float distanceToPlayer = sqrt((playerX - enemies[i].x) * (playerX - enemies[i].x) + (playerY - enemies[i].y) * (playerY - enemies[i].y));

		// Calculate the movement step size based on the distance
		float step = 0.000005 * distanceToPlayer; // Decrease step size

		float repulsionForceX = 0;
		float repulsionForceY = 0;

		// Calculate the repulsion force between the enemies
		for (int j = 0; j < enemies.size(); j++)
		{
			if (i == j) continue;

			float distanceToEnemy = sqrt((enemies[j].x - enemies[i].x) * (enemies[j].x - enemies[i].x) + (enemies[j].y - enemies[i].y) * (enemies[j].y - enemies[i].y));
			float force = 0.005 / distanceToEnemy; // Decrease force strength
			repulsionForceX += (enemies[i].x - enemies[j].x) * force;
			repulsionForceY += (enemies[i].y - enemies[j].y) * force;
		}

		// Move the enemy closer to the player, with a slight repulsion force between the enemies
		enemies[i].x += (playerX - enemies[i].x) * step + repulsionForceX;
		enemies[i].y += (playerY - enemies[i].y) * step + repulsionForceY;

		// Update the enemy's SDL_Rect
		enemies[i].enemyRects[0].x = (int)enemies[i].x;
		enemies[i].enemyRects[0].y = (int)enemies[i].y;
	}
}




void Enemy::deleteDeadEnemies()
{
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const enemy& e) { return e.dead; }), enemies.end());

	TOTAL_ENEMY_NUM = enemies.size();

	// If the number of enemies falls below a certain threshold, reset the enemy vectors
	if (enemies.size() < RESET_THRESHOLD)
	{
		reset();
	}
}