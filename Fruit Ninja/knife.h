#pragma once
#include "MyTexture.h"
#include <math.h>

extern SDL_Texture *blank;
class Knife {
private:
	SDL_Point point[6];
	int nextx, nexty;
	int miliiTime[5];  //刀光存在时间
	Mix_Chunk *swipe[7];
	Uint32 time,interval;
	bool playingMusic,ismove;
public:
	Knife();
	~Knife();
	inline int Length(int i);
	double getAngle(int i);
	void move(int x, int y);
	void update(Uint32 t);
	void render();
};