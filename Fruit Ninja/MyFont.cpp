#include "MyFont.h"
MyFont::MyFont(const std::string& text,int h):texture(NULL),width(0),height(0) {
	TextFont = TTF_OpenFont(text.c_str(), h);
}

MyFont::~MyFont() {
	//Deallocate
	free();
}

void MyFont::loadFromRenderedText(const std::string& text, SDL_Color TextColor) {

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(TextFont, text.c_str(), TextColor);
	//Create texture from surface pixels
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	//Get image dimensions
	width = textSurface->w;
	height = textSurface->h;

	//Get rid of old surface
	SDL_FreeSurface(textSurface);
}

void MyFont::free()
{
	//Free texture if it exists
	if (texture != NULL) SDL_DestroyTexture(texture);
	TTF_CloseFont(TextFont);
}

void MyFont::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture rgb
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void MyFont::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, int w) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad;
	if(w==0) renderQuad = { x, y, width,height };
	else renderQuad = { x, y, w,(int)(height*1.0 / width * w) };

	//Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int MyFont::getWidth() {
	return width;
}

int MyFont::getHeight() {
	return height;
}