#pragma once
#include "MyTexture.h"
#include "Knife.h"
#include "MyFont.h"
#include <math.h>
enum TYPE {
	APPLE,
	BANANA,
	PEACH,
	STRAWBERRY,
	WATERMELON,
	BOOM
};
enum INTERFACE {
	INTERFACE_QUIT,
	INTERFACE_MENU,
	INTERFACE_CLASSIC,
	INTERFACE_DOJO,
	INTERFACE_GAMEOVER
};
extern SDL_Renderer *renderer;
extern TTF_Font *font;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Mix_Chunk *chunk_throw;
extern INTERFACE interface;
extern SDL_Texture *blank;
extern int score,lose;
extern MyFont *scoreFont;

class Dot {
private:
	double posx[15], posy[15], velx[15], vely[15];
	int time;
	bool show;
	SDL_Texture *dotTexture;
public:
	Dot();
	void init(int x, int y,int type);
	void render();
	bool isShown();
	void free();
};

class Fruit {
protected:
	SDL_Texture *fruitTexture,*left,*right;  //水果图片（三张图片尺寸相同）
	int type;  //水果类型
	int width, height;  //水果宽度，高度
	bool show, cut;     //是否显示，是否被切
	double posx, posy, posx1, posx2,posy1,posy2,velx,velx1, vely, angle;
	Dot dot;
public:
	static const double FRUIT_ACCLERATE;
	static const double FRUIT_ROTATE_VEL;
	Fruit();
	~Fruit();
	void init();
	void init(int i);
	void setPosition(int x, int y);
	virtual void move(Uint32 milliSeconds);
	virtual void handleEvent(int x,int y,Knife &knife);
	bool handleEvent_menu(int x, int y);
	bool isShown();
	bool falledDown();
	int getWidth();
	int getHeight();
	virtual void render(SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void rotateAndRender(Uint32 t);
	void free();
};
extern Fruit fruit[10];

class Bomb :public Fruit {
public:
	Bomb();
	virtual void move(Uint32 milliSeconds);
	virtual bool handleEvent(int x, int y);
	virtual void render(SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
private:
	Uint32 interval;
};

struct Fruit_Material {
	void load();
	void free();
	//load image
	SDL_Texture *apple, *banana, *peach, *strawberry, *watermelon, *bomb, *apple1, *banana1, *peach1, *strawberry1, *watermelon1, *apple2, *banana2, *peach2, *strawberry2, *watermelon2, *shadow, *flash,*red_dot,*cyan_dot,*yellow_dot,*explodeLight;
	//image dimensions
	int apple_width, apple_height, banana_width, banana_height, peach_width, peach_height, strawberry_width, strawberry_height, watermelon_width, watermelon_height, boom_width, boom_height, shadow_width, shadow_height, flash_width, flash_height, explodeLight_width, explodeLight_height;
	//load sounds
	Mix_Chunk *splatter[2],*bomb_explode,*bomb_fuse;
};