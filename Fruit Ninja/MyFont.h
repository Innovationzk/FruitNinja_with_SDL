#pragma once
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

extern SDL_Renderer *renderer;
class MyFont {
private:
	SDL_Texture *texture;
	TTF_Font *TextFont;
	int width, height;
public:
	MyFont(const std::string& text,int h);
	~MyFont();
	int getWidth();
	int getHeight();
	void loadFromRenderedText(const std::string& text, SDL_Color TextColor);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE,int w=0);
	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
};
