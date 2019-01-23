#include "MyTexture.h"

MyTexture::MyTexture() :texture(NULL), mWidth(0), mHeight(0) {}

MyTexture::~MyTexture() {
	free();
}

int MyTexture::getHeight() {
	return mHeight;
}
int MyTexture::getWidth() {
	return mWidth;
}

void MyTexture::free(){
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool MyTexture::loadFromFile(const std::string& path) {
	free();
	SDL_Surface *tempsurface = NULL;
	tempsurface = IMG_Load(path.c_str());
	if (tempsurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		texture = SDL_CreateTextureFromSurface(renderer, tempsurface);
		if (texture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			mWidth = tempsurface->w;
			mHeight = tempsurface->h;
		}
		SDL_FreeSurface(tempsurface);
	}
	return texture!=NULL;
}

void MyTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, double zoom) {
	SDL_Rect dstRect = { x,y,mWidth,mHeight };
	if (clip != NULL) {
		dstRect.w = clip->w;
		dstRect.h = clip->h;
	}
	if (zoom) {
		dstRect.w = mWidth*zoom;
		dstRect.h = mHeight*zoom;
		dstRect.x = x + (mWidth - dstRect.w) / 2;
		dstRect.y = y + (mHeight - dstRect.h) / 2;
	}
	SDL_RenderCopyEx(renderer, texture, clip, &dstRect, angle, center, flip);
}
