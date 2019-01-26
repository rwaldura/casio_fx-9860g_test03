/*
 * THINGS TO KEEP IN MIND
 * bit 0 = off = white
 * bit 1 = on = black
 */

#include <stdlib.h>
#include "Sprite.h"

extern "C"
{
	#include "fxlib.h"
}

const int REFRESH_FREQUENCY = 50; // ms

const int SCREEN_WIDTH = IM_VRAM_WIDTH; // pixels
const int SCREEN_HEIGHT = IM_VRAM_HEIGHT; // pixels

enum Action
{
	NULL_ACTION,
	EXIT_ACTION, 
	MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
};

void draw_background(const Sprite* pattern)
{	
	for (int x = 0; x < SCREEN_WIDTH; x += pattern->width)
	{
		for (int y = 0; y < SCREEN_HEIGHT; y += pattern->height)
		{
			pattern->draw(x, y);
		}
	}
}

Action handle_keyboard()
{
	Action a = NULL_ACTION;

	if (IsKeyDown(KEY_CTRL_EXIT))
		return EXIT_ACTION;

	if (IsKeyDown(KEY_CTRL_UP))
		a = MOVE_UP;
	else if (IsKeyDown(KEY_CTRL_DOWN))
		a = MOVE_DOWN;

	if (IsKeyDown(KEY_CTRL_LEFT))
		a = MOVE_LEFT;
	else if (IsKeyDown(KEY_CTRL_RIGHT))
		a = MOVE_RIGHT;

	return a;
}

static void log(const unsigned char* mesg)
{
	static int line = 0;
	PrintMini(0, line, mesg, MINI_OVER);
	line += 6;
	unsigned int key; GetKey(&key);
}

/*
 * An object in the game: the ball, the paddle, the bricks.
 * The size of object can differ from the size of its sprite! 
 * This lets us accommodate shadows etc. around the object itself.
 */
class GameObject
{
public:
	int width, height;
	int x, y; // location in the game area
	int delta_x, delta_y;
	const Sprite* sprite;

	GameObject(int h, int w, const Sprite* s = 0, int xx = 0, int yy = 0) 
		: width(w), height(h), sprite(s), x(xx), y(yy) { }
};

extern "C" int test03_main(int isAppli, unsigned short optionNum)
{
	// load all the sprites, we're going to need them
	SpriteFactory* spriteFactory = new SpriteFactory();
	spriteFactory->load_all();

	// the ball 
	GameObject* ball = new GameObject(6, 6, spriteFactory->get(BLACK_BALL));
	ball->delta_x = 3;
	ball->delta_y = 2;

	bool quit = false;
	while (!quit)
    {
		Action action = handle_keyboard();
		switch (action)
		{
			case EXIT_ACTION:
				quit = true;
				break;
			
			case MOVE_UP:
				ball->delta_x = 0;
				ball->delta_y = -3;	// to move up, we must reduce y
				break;

			case MOVE_DOWN:
				ball->delta_x = 0;
				ball->delta_y = +3;
				break;

			case MOVE_LEFT:
				ball->delta_x = -3;
				ball->delta_y = 0;
				break;

			case MOVE_RIGHT:
				ball->delta_x = +3;
				ball->delta_y = 0;
				break;
		}

		// ball bounces off the edges of the screen
		if (ball->x <= 0)
		{
			ball->delta_x = +3;
			if (ball->delta_y == 0 && rand() % 3 == 0) ball->delta_y = +1;
		} 
		else if (ball->x >= SCREEN_WIDTH - ball->width) 
		{
			ball->delta_x = -3;			
			if (ball->delta_y == 0 && rand() % 3 == 0) ball->delta_y = +1;
		}

		// if ball goes in a straight line, randomize its bouncing
		if (ball->y <= 0) 
		{
			if (ball->delta_x == 0 && rand() % 3 == 0) ball->delta_x = +1;
			ball->delta_y = +1;
		}
		else if (ball->y >= SCREEN_HEIGHT - ball->height) 
		{
			if (ball->delta_x == 0 && rand() % 3 == 0) ball->delta_x = +1;
			ball->delta_y = -1;			
		}

		ball->x += ball->delta_x;
		ball->y += ball->delta_y;

		// refresh the screen
		draw_background(spriteFactory->get(GREY_PATTERN));
		ball->sprite->draw(ball->x, ball->y);
		Bdisp_PutDisp_DD();	

		// and wait
		Sleep(REFRESH_FREQUENCY);		
    }

    return 1;
}

