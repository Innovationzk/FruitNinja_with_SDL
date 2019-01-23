#include "MyTexture.h"
#include "Fruit.h"
#include "knife.h"
#include "MyFont.h"
#include <ctime>
#include <fstream>
#include <string>

const int SCREEN_WIDTH = 640;  //screen dimension
const int SCREEN_HEIGHT = 480;

int score,lose,bestScore;
INTERFACE interface = INTERFACE_MENU;
SDL_Window *window=NULL;
SDL_Renderer *renderer = NULL;
MyTexture background,img_score,gameOverTexture,circleNewGame,circleQuit, circleDojo,homeMask, ninja,homeDesc,xxx[3], xxxf[3], new_game_static, dojo_static, quit_static;
SDL_Texture *blank;
SDL_Event e;
Mix_Chunk *chunk_throw = NULL,*gameOver=NULL,*gameStart=NULL;
Mix_Music *music_menu_background = NULL,*music_menu=NULL;
Fruit fruit[10] = { Fruit(),Fruit(),Fruit(),Fruit(),Fruit(),Fruit(),Fruit(),Fruit(),Fruit(),Fruit() },menu_watermelon,menu_bomb,menu_peach;
Fruit_Material fruit_material;
MyFont *scoreFont = NULL, *bestScoreFont = NULL, *yourScoreIs;

bool init() {
	srand(time(0));
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) return false;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;
	if (TTF_Init() != 0) return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");   //Set texture filtering to linear
	window = SDL_CreateWindow("Fruit Ninja", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  //硬件加速与垂直同步

	return true;
}
void loadMedia() {
	SDL_Surface *temp = IMG_Load("sources/images/blank.png");
	blank = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	xxx[0].loadFromFile("sources/images/x.png");
	xxx[1].loadFromFile("sources/images/xx.png");
	xxx[2].loadFromFile("sources/images/xxx.png");
	xxxf[0].loadFromFile("sources/images/xf.png");
	xxxf[1].loadFromFile("sources/images/xxf.png");
	xxxf[2].loadFromFile("sources/images/xxxf.png");
	fruit_material.load();
	background.loadFromFile("sources/images/background.jpg");
	img_score.loadFromFile("sources/images/score.png");
	gameOverTexture.loadFromFile("sources/images/game-over.png");
	circleNewGame.loadFromFile("sources/images/new-game.png");
	circleQuit.loadFromFile("sources/images/quit.png");
	circleDojo.loadFromFile("sources/images/dojo.png");
	homeMask.loadFromFile("sources/images/home-mask.png");
	ninja.loadFromFile("sources/images/ninja.png");
	homeDesc.loadFromFile("sources/images/home-desc.png");
	new_game_static.loadFromFile("sources/images/new-game-static.png");
	dojo_static.loadFromFile("sources/images/dojo-static.png");
	quit_static.loadFromFile("sources/images/quit-static.png");
	menu_watermelon.init(4);
	menu_bomb.init(5);
	menu_peach.init(2);

	chunk_throw = Mix_LoadWAV("sources/sound/throw.wav");
	gameOver = Mix_LoadWAV("sources/sound/Game-over.ogg");
	gameStart = Mix_LoadWAV("sources/sound/Game-start.ogg");
	music_menu_background = Mix_LoadMUS("sources/sound/background.ogg");
	music_menu = Mix_LoadMUS("sources/sound/menu.ogg");

	scoreFont = new MyFont("sources/fonts/micross.ttf", 28);
	bestScoreFont = new MyFont("sources/fonts/micross.ttf", 18);
	yourScoreIs = new MyFont("sources/fonts/micross.ttf", 42);
}
void close() {
	background.free();
	img_score.free();
	gameOverTexture.free();
	circleQuit.free();
	circleNewGame.free();
	circleDojo.free();
	fruit_material.free();
	menu_watermelon.free();
	menu_bomb.free();
	menu_peach.free();
	homeMask.free();
	ninja.free();
	homeDesc.free();
	new_game_static.free();
	dojo_static.free();
	quit_static.free();
	SDL_DestroyTexture(blank);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_FreeChunk(chunk_throw);
	Mix_FreeMusic(music_menu_background);
	Mix_FreeMusic(music_menu);
	for (int i = 0; i < 10; i++) fruit[i].free();
	scoreFont->free();
	bestScoreFont->free();
	yourScoreIs->free();
	xxx[0].free();
	xxx[1].free();
	xxx[2].free();
	xxxf[0].free();
	xxxf[1].free();
	xxxf[2].free();

	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
}
void showMenu() {
	Mix_PlayMusic(music_menu, 0);
	Knife knife;
	int x, y;
	double circleAngle = 0;
	Uint32 lastTime = SDL_GetTicks(), nowTime,timeFlag=0;
	menu_peach.setPosition(95, 310);
	menu_watermelon.setPosition(285, 257);
	menu_bomb.setPosition(500, 335);
	while (interface) {
		nowTime = SDL_GetTicks();
		timeFlag += (nowTime - lastTime);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) interface = INTERFACE_QUIT;
			else if (SDL_GetMouseState(&x, &y)&SDL_BUTTON_LEFT) {
				knife.move(x, y);
				menu_peach.handleEvent_menu(x, y);
				menu_watermelon.handleEvent_menu(x, y);
				if (menu_bomb.handleEvent_menu(x, y)) {
					interface = INTERFACE_QUIT;
					return;
				}
			}
		}
		background.render(0, 0);
		knife.update(nowTime);

		if(timeFlag<400) homeMask.render(0, -183+timeFlag*0.4575);
		else homeMask.render(0, 0);
		if (timeFlag < 800) ninja.render(320, -380+timeFlag*0.525);
		else ninja.render(320, 40);
		if (timeFlag < 1200) homeDesc.render(-500+timeFlag*0.42, 150);
		else homeDesc.render(0, 150);
		if (timeFlag >= 1200 && timeFlag < 1700) {
			dojo_static.render(40, 250,NULL, 0, NULL, SDL_FLIP_NONE,  (timeFlag-1200)/500.0 );
			new_game_static.render(240, 200, NULL, 0, NULL, SDL_FLIP_NONE, (timeFlag - 1200) / 500.0);
			quit_static.render(460, 300, NULL, 0, NULL, SDL_FLIP_NONE, (timeFlag - 1200) / 500.0);
		}
		else if (timeFlag >= 1700) {
			circleDojo.render(40, 250,NULL,circleAngle -= 2);
			circleNewGame.render(240, 200, NULL, circleAngle);
			circleQuit.render(460, 300, NULL, circleAngle);
			menu_peach.rotateAndRender(nowTime - lastTime);
			menu_watermelon.rotateAndRender(nowTime - lastTime);
			menu_bomb.rotateAndRender(nowTime - lastTime);
			if (menu_peach.falledDown()) {
				interface = INTERFACE_DOJO;
				return;
			}
			if (menu_watermelon.falledDown()) {
				interface = INTERFACE_CLASSIC;
				return;
			}
		}

		knife.render();
		lastTime = nowTime;
		SDL_RenderPresent(renderer);
	}
}
void showClassicMode(){
	Mix_PlayMusic(music_menu_background, -1);
	Mix_PlayChannel(-1, gameStart, 0);
	Uint32 lastTime=SDL_GetTicks(), nowTime;
	Uint32 fruitInterval = SDL_GetTicks() + 2000;
	std::fstream file("sources/classicMode.txt", std::ios::in);
	file >> bestScore;
	file.close();
	int fruitNum = 0;
	int x, y;   //鼠标位置
	Knife knife;
	Bomb bomb;
	score = lose = 0;
	scoreFont->loadFromRenderedText("0", { 255,255,0,255 });
	bestScoreFont->loadFromRenderedText("best:"+std::to_string(bestScore), { 255,255,0,255 });
	while (interface) {
		background.render(0, 0);
		nowTime = SDL_GetTicks();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)  interface = INTERFACE_QUIT;
			else if (SDL_GetMouseState(&x, &y)& SDL_BUTTON(SDL_BUTTON_LEFT)) {
				knife.move(x, y);
				if (bomb.handleEvent(x, y)) {
					if (score > bestScore) {
						file.open("sources/classicMode.txt", std::ios::out);
						file << score;
						file.close();
					}
					interface = INTERFACE_GAMEOVER;
					for (int i = 0; i < 10; i++) fruit[i].free();
					return;
				}
				for (int i = 0; i < 10; i++) {
					fruit[i].handleEvent(x, y,knife);
				}
			}
		}
		knife.update(nowTime);
		if (nowTime > fruitInterval) {
			fruitInterval += 1500;
			fruitNum = rand() % 6 + 1;
			if (fruitNum > 5) fruitNum -= 5;
			else if (fruitNum > 3) fruitNum -= 3;
			for (int i = 0; i < 10 && fruitNum; i++) {
				if (fruit[i].isShown() == false) {
					fruit[i].init();
					fruitNum--;
				}
			}
			Mix_PlayChannel(-1, chunk_throw, 0);
		}
		for (int i = 0; i < 10; i++) {
			if (fruit[i].isShown()) {
				fruit[i].move(nowTime - lastTime);
				fruit[i].render();
			}
		}

		img_score.render(8, 9);
		scoreFont->render(43, 10);
		bestScoreFont->render(8, 50);
		if (lose == 0) {
			xxx[0].render(535, 9);
			xxx[1].render(565, 9);
			xxx[2].render(605, 9);
		}
		else if (lose == 1) {
			xxxf[0].render(535, 9);
			xxx[1].render(565, 9);
			xxx[2].render(605, 9);
		}
		else if (lose == 2) {
			xxxf[0].render(535, 9);
			xxxf[1].render(565, 9);
			xxx[2].render(605, 9);
		}
		else {
			xxxf[0].render(535, 9);
			xxxf[1].render(565, 9);
			xxxf[2].render(605, 9);
			interface = INTERFACE_GAMEOVER;
			if (score > bestScore) {
				file.open("sources/classicMode.txt", std::ios::out);
				file << score;
				file.close();
			}
			for (int i = 0; i < 10; i++) fruit[i].free();
			lose = 0;
			return;
		}

		bomb.move(nowTime - lastTime);
		bomb.render();
		knife.render();
		lastTime = nowTime;
		SDL_RenderPresent(renderer);
	}
	Mix_HaltMusic();
}
void showGameOver() {
	Mix_PlayChannel(-1, gameOver, 0);
	int temp = 40;    //控制gameover由上至下进入
	double circleAngle = 0;
	Uint32 lastTime = SDL_GetTicks(), nowTime;
	yourScoreIs->loadFromRenderedText("Your score is: " + std::to_string(score), {255,255,0,255});
	scoreFont->loadFromRenderedText(std::to_string(score), { 255,255,0,255 });
	bestScoreFont->loadFromRenderedText("best:" + std::to_string(bestScore), { 255,255,0,255 });
	Knife knife;
	int x, y;
	menu_watermelon.setPosition(87, 305);
	menu_bomb.setPosition(443, 335);
	while (interface) {
		nowTime = SDL_GetTicks();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) interface = INTERFACE_QUIT;
			else if (SDL_GetMouseState(&x, &y)&SDL_BUTTON_LEFT) {
				knife.move(x, y);
				menu_watermelon.handleEvent_menu(x, y);
				if (menu_bomb.handleEvent_menu(x, y)) {
					interface = INTERFACE_QUIT;
					return;
				}
			}
		}
		background.render(0, 0);
		knife.update(nowTime);
		if (temp != 0) {
			gameOverTexture.render(75, 80 - temp-- * 2);
			yourScoreIs->render(170 + temp * 1, 190 + temp, NULL, 0, NULL, SDL_FLIP_NONE, 290-temp * 3);
		}
		else {
			gameOverTexture.render(75, 80);
			yourScoreIs->render(170, 190);
		}

		circleNewGame.render(40, 250,NULL,circleAngle-=2);
		circleQuit.render(400, 300,NULL,circleAngle);
		menu_watermelon.rotateAndRender(nowTime-lastTime);
		menu_bomb.rotateAndRender(nowTime-lastTime);
		knife.render();

		if (menu_watermelon.falledDown()) {
			interface = INTERFACE_MENU;
			return;
		}
		img_score.render(8, 9);
		scoreFont->render(43, 10);
		bestScoreFont->render(8, 50);
		if (lose == 0) {
			xxx[0].render(535, 9);
			xxx[1].render(565, 9);
			xxx[2].render(605, 9);
		}
		else if (lose == 1) {
			xxxf[0].render(535, 9);
			xxx[1].render(565, 9);
			xxx[2].render(605, 9);
		}
		else if (lose == 2) {
			xxxf[0].render(535, 9);
			xxxf[1].render(565, 9);
			xxx[2].render(605, 9);
		}
		else if(lose>2){
			xxxf[0].render(535, 9);
			xxxf[1].render(565, 9);
			xxxf[2].render(605, 9);
		}
		lastTime = nowTime;
		SDL_RenderPresent(renderer);
	}
}
void showDojoMode() {
	Mix_PlayMusic(music_menu_background, -1);
	Mix_PlayChannel(-1, gameStart, 0);
	Uint32 lastTime = SDL_GetTicks(), nowTime, timeFlag = 0, leftTime = 30;
	Uint32 fruitInterval = SDL_GetTicks() + 2000,timeInterval=1000;
	std::fstream file("sources/dojoMode.txt", std::ios::in);
	file >> bestScore;
	file.close();
	int fruitNum = 0;
	int x, y;   //鼠标位置
	Knife knife;
	MyFont timeFont("sources/fonts/micross.ttf", 28);
	timeFont.loadFromRenderedText(std::to_string(30), { 255,255,0,255 });
	score=0;
	scoreFont->loadFromRenderedText("0", { 255,255,0,255 });
	bestScoreFont->loadFromRenderedText("best:" + std::to_string(bestScore), { 255,255,0,255 });
	while (interface) {
		background.render(0, 0);
		nowTime = SDL_GetTicks();
		timeFlag += (nowTime - lastTime);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)  interface = INTERFACE_QUIT;
			else if (SDL_GetMouseState(&x, &y)& SDL_BUTTON(SDL_BUTTON_LEFT)) {
				knife.move(x, y);
				for (int i = 0; i < 10; i++) {
					fruit[i].handleEvent(x, y, knife);
				}
			}
		}
		knife.update(nowTime);
		if (nowTime > fruitInterval) {
			fruitInterval += 1500;
			fruitNum = rand() % 6 + 1;
			if (fruitNum > 5) fruitNum -= 5;
			else if (fruitNum > 3) fruitNum -= 3;
			for (int i = 0; i < 10; i++) {
				if (fruit[i].isShown() == false && fruitNum) {
					fruit[i].init();
					fruitNum--;
				}
			}
			Mix_PlayChannel(-1, chunk_throw, 0);
		}
		for (int i = 0; i < 10; i++) {
			if (fruit[i].isShown()) {
				fruit[i].move(nowTime - lastTime);
				fruit[i].render();
			}
		}
		if (timeFlag > timeInterval) {
			timeInterval += 1000;
			if (leftTime-- == 0) {
				interface = INTERFACE_GAMEOVER;
				lose = -1;
				for (int i = 0; i < 10; i++)  fruit[i].free();
				if (score > bestScore) {
					file.open("sources/dojoMode.txt", std::ios::out);
					file << score;
					file.close();
				}
				return;
			}
			timeFont.loadFromRenderedText(std::to_string(leftTime), { 255,255,0,255 });
		}

		img_score.render(8, 9);
		scoreFont->render(43, 10);
		bestScoreFont->render(8, 50);
		timeFont.render(600, 10);

		knife.render();
		lastTime = nowTime;
		SDL_RenderPresent(renderer);
	}
	Mix_HaltMusic();
}
int main(int argc, char* args[]) {
	if (!init()) return -1;
	loadMedia();
	while (interface) {
		if (interface == INTERFACE_MENU) showMenu();
		else if (interface == INTERFACE_CLASSIC) showClassicMode();
		else if (interface == INTERFACE_DOJO) showDojoMode();
		else if (interface == INTERFACE_GAMEOVER) showGameOver();
	}
	close();
	return 0;
}