#include "Fruit.h"
#include <string>

const double Fruit::FRUIT_ACCLERATE = 600;
const double Fruit::FRUIT_ROTATE_VEL = 3;
extern Fruit_Material fruit_material;

Fruit::Fruit() :fruitTexture(NULL), left(NULL), right(NULL), width(0), height(0), type(0), show(false), cut(false), posx(0), posy(0), posx1(0), posx2(0), velx(0),velx1(0), vely(0), angle(0) {}

Fruit::~Fruit() {
	free();
}

void Fruit::init(){
	show = true;
	type = rand() % 5;
	switch (type) {
	case 0:
		fruitTexture = fruit_material.apple;
		left = fruit_material.apple1;
		right = fruit_material.apple2;
		width = fruit_material.apple_width;
		height = fruit_material.apple_height;
		break;
	case 1:
		fruitTexture = fruit_material.banana;
		left = fruit_material.banana1;
		right = fruit_material.banana2;
		width = fruit_material.banana_width;
		height = fruit_material.banana_height;
		break;
	case 2:
		fruitTexture = fruit_material.peach;
		left = fruit_material.peach1;
		right = fruit_material.peach2;
		width = fruit_material.peach_width;
		height = fruit_material.peach_height;
		break;
	case 3:
		fruitTexture = fruit_material.strawberry;
		left = fruit_material.strawberry1;
		right = fruit_material.strawberry2;
		width = fruit_material.strawberry_width;
		height = fruit_material.strawberry_height;
		break;
	case 4:
		fruitTexture = fruit_material.watermelon;
		left = fruit_material.watermelon1;
		right = fruit_material.watermelon2;
		width = fruit_material.watermelon_width;
		height = fruit_material.watermelon_height;
		break;
	}
	posx = rand() % (SCREEN_WIDTH / 4*3) + SCREEN_WIDTH / 8;
	posy = SCREEN_HEIGHT + height / 2;
	if (posx < SCREEN_WIDTH / 8 * 3) velx = 120;
	else if (posx < SCREEN_WIDTH / 2) velx = 70;
	else if (posx < SCREEN_WIDTH / 2 * 5) velx = -70;
	else velx = -120;
	velx1 = velx < 0 ? -velx : velx;
	vely = -600-rand()%120;
	show = true;
	angle = 6-rand() % 120;
}

void Fruit::init(int i) {
	show = true;
	type = i;
	switch (i) {
	case 2:
		fruitTexture = fruit_material.peach;
		left = fruit_material.peach1;
		right = fruit_material.peach2;
		width = fruit_material.peach_width;
		height = fruit_material.peach_height;
		break;
	case 4:
		fruitTexture = fruit_material.watermelon;
		left = fruit_material.watermelon1;
		right = fruit_material.watermelon2;
		width = fruit_material.watermelon_width;
		height = fruit_material.watermelon_height;
		break;
	case 5:
		fruitTexture = fruit_material.bomb;
		width = fruit_material.boom_width;
		height = fruit_material.boom_height;
		break;
	}
	velx = 100;
	vely = 0;
	angle = 0;
}

inline int Fruit::getHeight() {
	return height;
}
inline int Fruit::getWidth() {
	return width;
}
bool Fruit::isShown() {
	return show;
}

void Fruit::move(Uint32 milliSeconds) {
	if (cut == false) {
		posx += velx * milliSeconds / 1000;
		posy += vely * milliSeconds / 1000;
		if (angle >= 0) angle += FRUIT_ROTATE_VEL;
		else angle -= FRUIT_ROTATE_VEL;
		if (posy > SCREEN_HEIGHT + height / 2) {
			free();
			lose++;
		}
	}
	else {
		posx1 -= velx1 * milliSeconds / 1000;
		posx2 += velx1 * milliSeconds / 1000;
		posy1 += vely * milliSeconds / 1000;
		posy2 = posy1;
		angle += 1;
		if (posy1 > SCREEN_HEIGHT + height / 2) free();
	}
	vely += FRUIT_ACCLERATE * milliSeconds*1.0 / 1000;
}

void Fruit::setPosition(int x, int y) {
	posx = x; posy = y;
}
void Fruit::handleEvent(int x,int y,Knife &knife) {
	if (show && !cut) {
		if ((x - posx)*(x - posx) + (y - posy)*(y - posy) <= 33 * 33) {
			dot.init(x, y,type);
			SDL_Rect rect = { posx - 150,posy - 8,300,15 };
			double tempangle = knife.getAngle(5);
			SDL_RenderCopyEx(renderer, fruit_material.flash, NULL, &rect, tempangle, NULL, SDL_FLIP_NONE);
			Mix_PlayChannel(-1,fruit_material.splatter[rand() % 2], 0);
			cut = true;
			posx1 = posx;
			posx2 = posx;
			posy1 = posy2 = posy;
			angle = 0;
			vely = 0;
			score++;
			scoreFont->loadFromRenderedText(std::to_string(score), { 255,255,0,255 });
		}
	}
}
bool Fruit::handleEvent_menu(int x, int y) {
	if (show && !cut) {
		if ((x - posx-width/2)*(x - posx-width/2) + (y - posy-height/2)*(y - posy-height/2) <= 35 * 35) {
			if (type != 5) {
				Mix_PlayChannel(-1, fruit_material.splatter[rand() % 2], 0);
				dot.init(x, y, type);
				cut = true;
				posx1 = posx;
				posx2 = posx;
				posy1 = posy2 = posy;
				angle = 0;
				velx1 = 100;
				vely = 0;
			}
			return true;
		}
	}
	return false;
}
void Fruit::free() {
	if (fruitTexture != NULL) {
		fruitTexture = NULL; left = NULL; right = NULL;
		dot.free();
		show = cut = false;
	}
}

void Fruit::render(SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip) {
	if (cut == false) {
		SDL_Rect dstRect = { posx - 50,posy +17,106,77 };
		SDL_RenderCopyEx(renderer, fruit_material.shadow, clip, &dstRect, 0, center, flip);
		dstRect = { (int)posx - width / 2,(int)posy - height / 2,width,height };
		SDL_RenderCopyEx(renderer, fruitTexture, clip, &dstRect, angle, center, flip);
	}
	else {
		SDL_Rect dstRect = { posx1 - width / 2,posy1 - height / 2,width,height };
		SDL_RenderCopyEx(renderer, left, clip, &dstRect, -angle, center, flip);
		dstRect.x = posx2 - width / 2;
		dstRect.y = posy2 - width / 2;
		SDL_RenderCopyEx(renderer, right, clip, &dstRect, angle, center, flip);
	}
	if (dot.isShown()) dot.render();
}

void Fruit::rotateAndRender(Uint32 t) {
	if (cut == false) {
		SDL_Rect dstRect = { posx,posy,width,height };
		SDL_RenderCopyEx(renderer, fruitTexture, NULL, &dstRect, angle += 2, NULL, SDL_FLIP_NONE);
	}
	else {
		posx1 -= velx1 * t / 1000;
		posx2 += velx1 * t / 1000;
		vely += FRUIT_ACCLERATE*t/1500.0;
		posy1 += vely * t / 1000;
		posy2 = posy1;
		SDL_Rect dstRect = { posx1,posy1,width,height };
		SDL_RenderCopyEx(renderer, left, NULL, &dstRect, angle -= 1, NULL, SDL_FLIP_NONE);
		dstRect.x = posx2;
		dstRect.y = posy2;
		SDL_RenderCopyEx(renderer, right, NULL, &dstRect, -angle, NULL, SDL_FLIP_NONE);
	}
	if (dot.isShown()) dot.render();
}

bool Fruit::falledDown() {
	if (cut&&posy1 > SCREEN_HEIGHT) {
		posy1 = 0;
		cut = false;
		return true;
	}
	return false;
}

Dot::Dot() :show(false) {}
void Dot::free() { show = false; }
bool Dot::isShown() { return show; }
void Dot::init(int x, int y,int type) {
	switch(type){
	case 0:
		dotTexture = fruit_material.cyan_dot;
		break;
	case 1:
		dotTexture = fruit_material.yellow_dot;
		break;
	case 2:
		dotTexture = fruit_material.yellow_dot;
		break;
	case 3:
		dotTexture = fruit_material.red_dot;
		break;
	case 4:
		dotTexture = fruit_material.red_dot;
		break;
	}
	show = true;
	for (int i = 0; i < 15; i++) {
		posx[i] = x - 5 + rand() % 10;
		posy[i] = y - 5 + rand() % 10;
		velx[i] = -5 + rand() % 10;
		vely[i] = -5 + rand() % 10;
	}
	time = 30;
}
void Dot::render() {
	if (show) {
		if (time-- == 0) show = false;
		SDL_Rect dstRect = { 0,0,20,20 };
		SDL_Rect srcRect;
		if (time >= 24) srcRect = { 0,0,20,20 };
		else if (time >= 18) srcRect = { 20,0,20,20 };
		else if (time >= 12) srcRect = { 40,0,20,20 };
		else if (time >= 6) srcRect = { 60,0,20,20 };
		else srcRect = { 80,0,20,20 };
		for (int i = 0; i < 15; i++) {
			posx[i] += velx[i];
			posy[i] += vely[i];
			dstRect.x = posx[i];
			dstRect.y = posy[i];
			SDL_RenderCopy(renderer, dotTexture, &srcRect, &dstRect);
		}
	}
}
Bomb::Bomb() :Fruit(), interval(3000) { 
	fruitTexture = fruit_material.bomb;
	width = fruit_material.boom_width;
	height = fruit_material.boom_height;
}
void Bomb::move(Uint32 milliSeconds) {
	if (show == true) {
		posx += velx*milliSeconds/1000.0;
		posy += vely*milliSeconds/1000.0;
		vely += FRUIT_ACCLERATE * milliSeconds / 1000.0;
		if (angle > 0) angle += FRUIT_ROTATE_VEL;
		else angle -= FRUIT_ROTATE_VEL;
		if (posy - height / 2 > SCREEN_HEIGHT) show = false;
	}
	else {
		interval += milliSeconds;
		if (interval > 7000) {
			show = true;
			interval =0;
			posx = rand() % (SCREEN_WIDTH / 4 * 3) + SCREEN_WIDTH / 8;
			posy = SCREEN_HEIGHT + height / 2;
			if (posx < SCREEN_WIDTH / 8 * 3) velx = 180;
			else if (posx < SCREEN_WIDTH / 2) velx = 95;
			else if (posx < SCREEN_WIDTH / 2 * 5) velx = -95;
			else velx = -180;
			vely = -500 - rand() % 200;
			angle = 6 - rand() % 120;
		}
	}
}

bool Bomb::handleEvent(int x, int y) {
	if (show && (x - posx)*(x - posx) + (y - posy)*(y - posy) <= 33 * 33) {
		Mix_PlayChannel(-1, fruit_material.bomb_explode, 0);
		SDL_Rect dstRect = { (int)posx - width / 2,(int)posy - height / 2,width,height };
		SDL_RenderCopyEx(renderer, fruitTexture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);
		dstRect = { (int)posx,(int)posy,fruit_material.explodeLight_width,fruit_material.explodeLight_height };
		SDL_Point pt = { 0,0 };
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 0,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 120,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 270,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 90,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 240,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 150,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 330,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 30,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 210,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 60,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 180,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_RenderCopyEx(renderer, fruit_material.explodeLight, NULL, &dstRect, 300,&pt,SDL_FLIP_NONE);
		SDL_Delay(90);
		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(1500);
		return true;
	}
	return false;
}

void Bomb::render(SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip) {
	if (show) {
		SDL_Rect dstRect = { posx - 50,posy + 17,106,77 };
		SDL_RenderCopyEx(renderer, fruit_material.shadow, clip, &dstRect, 0, center, flip);
		dstRect = { (int)posx - width / 2,(int)posy - height / 2,width,height };
		SDL_RenderCopyEx(renderer, fruitTexture, clip, &dstRect, angle, center, flip);
	}
}

void Fruit_Material::load() {
	//load apple
	SDL_Surface *tempsurface = IMG_Load("sources/images/apple.png");
	SDL_Surface *tempsurface1 = IMG_Load("sources/images/apple-1.png");
	SDL_Surface *tempsurface2 = IMG_Load("sources/images/apple-2.png");
	apple = SDL_CreateTextureFromSurface(renderer, tempsurface);
	apple1 = SDL_CreateTextureFromSurface(renderer, tempsurface1);
	apple2 = SDL_CreateTextureFromSurface(renderer, tempsurface2);
	apple_width = tempsurface->w;
	apple_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	SDL_FreeSurface(tempsurface1);
	SDL_FreeSurface(tempsurface2);
	//load banana
	tempsurface = IMG_Load("sources/images/banana.png");
	tempsurface1 = IMG_Load("sources/images/banana-1.png");
	tempsurface2 = IMG_Load("sources/images/banana-2.png");
	banana = SDL_CreateTextureFromSurface(renderer, tempsurface);
	banana1 = SDL_CreateTextureFromSurface(renderer, tempsurface1);
	banana2 = SDL_CreateTextureFromSurface(renderer, tempsurface2);
	banana_width = tempsurface->w;
	banana_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	SDL_FreeSurface(tempsurface1);
	SDL_FreeSurface(tempsurface2);
	//load peach
	tempsurface = IMG_Load("sources/images/peach.png");
	tempsurface1 = IMG_Load("sources/images/peach-1.png");
	tempsurface2 = IMG_Load("sources/images/peach-2.png");
	peach = SDL_CreateTextureFromSurface(renderer, tempsurface);
	peach1 = SDL_CreateTextureFromSurface(renderer, tempsurface1);
	peach2 = SDL_CreateTextureFromSurface(renderer, tempsurface2);
	peach_width = tempsurface->w;
	peach_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	SDL_FreeSurface(tempsurface1);
	SDL_FreeSurface(tempsurface2);
	//load strawberry
	tempsurface = IMG_Load("sources/images/strawberry.png");
	tempsurface1 = IMG_Load("sources/images/strawberry-1.png");
	tempsurface2 = IMG_Load("sources/images/strawberry-2.png");
	strawberry = SDL_CreateTextureFromSurface(renderer, tempsurface);
	strawberry1 = SDL_CreateTextureFromSurface(renderer, tempsurface1);
	strawberry2 = SDL_CreateTextureFromSurface(renderer, tempsurface2);
	strawberry_width = tempsurface->w;
	strawberry_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	SDL_FreeSurface(tempsurface1);
	SDL_FreeSurface(tempsurface2);
	//load watermelon
	tempsurface = IMG_Load("sources/images/watermelon.png");
	tempsurface1 = IMG_Load("sources/images/watermelon-1.png");
	tempsurface2 = IMG_Load("sources/images/watermelon-2.png");
	watermelon = SDL_CreateTextureFromSurface(renderer, tempsurface);
	watermelon1 = SDL_CreateTextureFromSurface(renderer, tempsurface1);
	watermelon2 = SDL_CreateTextureFromSurface(renderer, tempsurface2);
	watermelon_width = tempsurface->w;
	watermelon_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	SDL_FreeSurface(tempsurface1);
	SDL_FreeSurface(tempsurface2);
	//load boom
	tempsurface = IMG_Load("sources/images/bomb.png");
	bomb = SDL_CreateTextureFromSurface(renderer, tempsurface);
	boom_width = tempsurface->w;
	boom_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	//load shadow
	tempsurface = IMG_Load("sources/images/shadow.png");
	shadow = SDL_CreateTextureFromSurface(renderer, tempsurface);
	shadow_width = tempsurface->w;
	shadow_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	//load flash
	tempsurface = IMG_Load("sources/images/flash.png");
	flash = SDL_CreateTextureFromSurface(renderer, tempsurface);
	flash_width = tempsurface->w;
	flash_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);
	//load red-dot
	tempsurface = IMG_Load("sources/images/red-dot.png");
	red_dot = SDL_CreateTextureFromSurface(renderer, tempsurface);
	SDL_FreeSurface(tempsurface);
	//load cyan-dot
	tempsurface = IMG_Load("sources/images/cyan-dot.png");
	cyan_dot = SDL_CreateTextureFromSurface(renderer, tempsurface);
	SDL_FreeSurface(tempsurface);
	//load yellow-dot
	tempsurface = IMG_Load("sources/images/yellow-dot.png");
	yellow_dot = SDL_CreateTextureFromSurface(renderer, tempsurface);
	SDL_FreeSurface(tempsurface);
	//load explode light
	tempsurface = IMG_Load("sources/images/explode-light.png");
	explodeLight = SDL_CreateTextureFromSurface(renderer, tempsurface);
	explodeLight_width = tempsurface->w;
	explodeLight_height = tempsurface->h;
	SDL_FreeSurface(tempsurface);

	//load sound
	splatter[0] = Mix_LoadWAV("sources/sound/Splatter-Medium-1.ogg");
	splatter[1] = Mix_LoadWAV("sources/sound/Splatter-Medium-2.ogg");
	bomb_explode = Mix_LoadWAV("sources/sound/Bomb-explode.ogg");
	bomb_fuse = Mix_LoadWAV("sources/sound/Bomb-Fuse.ogg");
}

void Fruit_Material::free() {
	SDL_DestroyTexture(apple);
	SDL_DestroyTexture(apple1);
	SDL_DestroyTexture(apple2);
	SDL_DestroyTexture(banana);
	SDL_DestroyTexture(banana1);
	SDL_DestroyTexture(banana2);
	SDL_DestroyTexture(peach);
	SDL_DestroyTexture(peach1);
	SDL_DestroyTexture(peach2);
	SDL_DestroyTexture(strawberry);
	SDL_DestroyTexture(strawberry1);
	SDL_DestroyTexture(strawberry2);
	SDL_DestroyTexture(watermelon);
	SDL_DestroyTexture(watermelon1);
	SDL_DestroyTexture(watermelon2);
	SDL_DestroyTexture(bomb);
	SDL_DestroyTexture(shadow);
	SDL_DestroyTexture(flash);
	SDL_DestroyTexture(red_dot);
	SDL_DestroyTexture(cyan_dot);
	SDL_DestroyTexture(yellow_dot);
	SDL_DestroyTexture(explodeLight);
	Mix_FreeChunk(splatter[0]);
	Mix_FreeChunk(splatter[1]);
	Mix_FreeChunk(bomb_explode);
	Mix_FreeChunk(bomb_fuse);
}