#pragma once
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

extern SDL_Renderer *renderer;
class MyTexture {
private:
	SDL_Texture *texture;
	int mWidth, mHeight;
public:
	MyTexture();
	~MyTexture();
	int getWidth();
	int getHeight();
	bool loadFromFile(const std::string& path);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, double zoom=0);
	void free();	
};
