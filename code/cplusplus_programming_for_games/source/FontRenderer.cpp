#include "FontRenderer.h"

FontRenderer::FontRenderer(SDL_Renderer* sdlRenderer)
{
	renderer = sdlRenderer;
}
void FontRenderer::init()
{
	int init = TTF_Init();
	if (init != 0) {
		std::cout << SDL_GetError() << std::endl;
	}
	//used font for font renderig
	font = TTF_OpenFont("assets/OpenSans-Regular.ttf", 12);
	if (font == NULL) {
		std::cout << "cant load font: " << SDL_GetError() << std::endl;
	}
}
void FontRenderer::clean()
{
	//Free up all the resources we used to render
	TTF_CloseFont(font);
}
void FontRenderer::render(std::string text, int posX, int posY, int size)
{
	SDL_Color colour = SDL_Color();
	colour.r = 120; //Red
	colour.g = 0; //Green
	colour.b = 60; //Blue
	colour.a = 225; //Transparanet

	//Creates an image representing the text that we input
	SDL_Surface* textImage = TTF_RenderText_Solid(font, text.c_str(), colour);
	//Once we have an image, we can render it the same way we do with our player
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textImage);
	SDL_Rect portion;
	portion.x = posX;
	portion.y = posY;
	portion.w = 100;
	portion.h = 50;

	//RenderCopy should look familiar, look at Player.cpp
	SDL_RenderCopy(renderer, texture, NULL, &portion);


	//Since text is rendered, we don't need texture nor image so it can be released
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textImage);
}