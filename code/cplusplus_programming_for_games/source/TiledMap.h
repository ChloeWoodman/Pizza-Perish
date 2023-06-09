#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <array>
#include <fstream>

class TiledMap
{
public:
	TiledMap(SDL_Renderer* sdlRenderer, std::string tiledMapFilename);

	void init();
	void clean();
	void render();

private:
	const int sourceTileSizePx = 70;
	const int tileWidth = 6;
	const int tileHeight = 7;

	//setting up tile values
	std::string filename;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int windowWidth = 800, windowHeight = 600;
	//width and height set for tilemap
	const static int mapWidth = 4;
	const static int mapHeight = 4;
	//reading tile from file
	std::array<std::array <int, mapWidth>, mapHeight> map;
};



