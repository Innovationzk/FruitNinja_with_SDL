#include "knife.h"

Knife::Knife():playingMusic(false),time(0),ismove(false) {
	interval = SDL_GetTicks();
	for (int i = 0; i < 6; i++) point[i].x = point[i].y = -1;
	swipe[0] = Mix_LoadWAV("sources/sound/Sword-swipe-1.ogg");
	swipe[1] = Mix_LoadWAV("sources/sound/Sword-swipe-2.ogg");
	swipe[2] = Mix_LoadWAV("sources/sound/Sword-swipe-3.ogg");
	swipe[3] = Mix_LoadWAV("sources/sound/Sword-swipe-4.ogg");
	swipe[4] = Mix_LoadWAV("sources/sound/Sword-swipe-5.ogg");
	swipe[5] = Mix_LoadWAV("sources/sound/Sword-swipe-6.ogg");
	swipe[6] = Mix_LoadWAV("sources/sound/Sword-swipe-7.ogg");
}
int Knife::Length(int i) {
	return (int)sqrt((point[i].x - point[i - 1].x)*(point[i].x - point[i - 1].x) + (point[i].y - point[i - 1].y)*(point[i].y - point[i - 1].y));
}
void Knife::move(int x, int y) {
	nextx = x;
	nexty = y;
	ismove = true;
}
void Knife::update(Uint32 t) {
	if(t-interval>20) {
		interval = t;
		for (int i = 0; i < 5; i++) {
			point[i].x = point[i + 1].x;
			point[i].y = point[i + 1].y;
		}
		if (ismove) {
			ismove = false;
			point[5].x = nextx;
			point[5].y = nexty;
			if (playingMusic == false) {
				if (point[0].x != -1 && point[5].x != -1 && (point[5].x - point[0].x)*(point[5].x - point[0].x) + (point[5].y - point[0].y)*(point[5].y - point[0].y) > 300 * 300) {
					Mix_PlayChannel(-1,swipe[rand() % 7], 0);
					playingMusic = true;
					time = SDL_GetTicks();
				}
			}
			else if (SDL_GetTicks() - time > 2000) playingMusic = false;
		}
		else {
			point[5].x = -1;
			point[5].y = -1;
		}
	}
}
double Knife::getAngle(int i) {
	return atan((point[i].y - point[i-1].y)*1.0 / (point[i].x - point[i - 1].x)) * 180 / 3.1415;
}
void Knife::render() {
	SDL_Rect rect;
	double angle;
	int length = 0;
	if (point[0].x != -1&&point[1].x!=-1) {
		angle = getAngle(1);
		length = Length(1);
		rect = { (point[1].x + point[0].x -length)/2 ,(point[1].y + point[0].y) / 2,length,2};
		SDL_RenderCopyEx(renderer, blank, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	}
	if (point[1].x != -1&&point[2].x!=-1) {
		angle = atan((point[2].y - point[1].y)*1.0 / (point[2].x - point[1].x))*180/3.1415;
		length = Length(2);
		rect = { (point[2].x + point[1].x -length)/2 ,(point[2].y + point[1].y) / 2,length,4};
		SDL_RenderCopyEx(renderer, blank, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	}
	if (point[2].x != -1&& point[3].x != -1) {
		angle = atan((point[3].y - point[2].y)*1.0 / (point[3].x - point[2].x))*180/3.1415;
		length = Length(3);
		rect = { (point[3].x + point[2].x -length)/2 ,(point[3].y + point[2].y) / 2,length,6};
		SDL_RenderCopyEx(renderer, blank, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	}
	if (point[3].x != -1&& point[4].x != -1) {
		angle = atan((point[4].y - point[3].y)*1.0 / (point[4].x - point[3].x))*180/3.1415;
		length = Length(4);
		rect = { (point[4].x + point[3].x -length)/2 ,(point[4].y + point[3].y) / 2,length,8};
		SDL_RenderCopyEx(renderer, blank, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	}
	if (point[4].x != -1&&point[5].x!=-1) {
		angle = atan((point[5].y - point[4].y)*1.0 / (point[5].x - point[4].x))*180/3.1415;
		rect = { (point[5].x + point[4].x -length)/2 ,(point[5].y + point[4].y) / 2,length,10};
		SDL_RenderCopyEx(renderer, blank, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
	}

}
Knife::~Knife() {
	for(int i=0;i<7;i++) Mix_FreeChunk(swipe[i]);
}