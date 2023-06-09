#pragma once
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <iostream>
#include <SDL_image.h>

class FontRenderer
{
public:
	FontRenderer(SDL_Renderer* sdlRenderer);
	~FontRenderer() { clean(); };
	void init();
	void render(std::string text, int posX, int posY, int size);
	void clean();
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
};

