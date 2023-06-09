#include "TiledMap.h"

TiledMap::TiledMap(SDL_Renderer* sdlRenderer, std::string tiledMapFilename)
{
	renderer = sdlRenderer;
	filename = tiledMapFilename;
}

void TiledMap::init()
{
	SDL_Surface* image = IMG_Load(filename.c_str());

	if (image != NULL)
	{
		//making texture from imported image
		texture = SDL_CreateTextureFromSurface(renderer, image);
		SDL_FreeSurface(image);
	}

	else
	{
		//if import tilemap fail
		printf("no");
	}


	//opening tilemap from notepad
	std::ifstream file;
	file.open("assets/tileMap.txt", std::ios::in);

	if (!file)
	{
		std::cout << "no file" << std::endl;
		return;
	}

	int tile;

	//reading tilemap
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			file >> tile;
			std::cout << tile << ' ';
			map[i][j] = tile;
		}

		std::cout << "\n";
	}

	file.close();

}

//clears tilemap
void TiledMap::clean()
{
	SDL_DestroyTexture(texture);
}

//rendering of tilemaps
void TiledMap::render() {
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			SDL_Rect sourceRect;
			sourceRect.x = (map[i][j] % 6) * sourceTileSizePx;
			sourceRect.y = (map[i][j] / 7) * sourceTileSizePx;
			sourceRect.h = sourceTileSizePx;
			sourceRect.w = sourceTileSizePx;

			SDL_Rect renderRect;
			//set the size of tiles
			renderRect.x = j * (windowWidth / 4);
			renderRect.y = i * (windowHeight / 4);
			//set the size of window
			renderRect.h = windowHeight / 4;
			renderRect.w = windowWidth / 4;

			SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
		}
	}
}