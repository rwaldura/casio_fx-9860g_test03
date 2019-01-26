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

extern "C" int test03_main(int isAppli, unsigned short optionNum)
{
	SpriteFactory* spriteFactory = new SpriteFactory();
	spriteFactory->load_all();

	const Sprite* ball = spriteFactory->get(BLACK_BALL);
	int ball_x = SCREEN_WIDTH / 2;
	int ball_y = SCREEN_HEIGHT / 2;
	int delta_x = 2;
	int delta_y = 1;

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
				delta_x = 0;
				delta_y = -2;	// to move up, we must reduce y
				break;

			case MOVE_DOWN:
				delta_x = 0;
				delta_y = +2;
				break;

			case MOVE_LEFT:
				delta_x = -2;
				delta_y = 0;
				break;

			case MOVE_RIGHT:
				delta_x = +2;
				delta_y = 0;
				break;
		}

		// ball bounces off the edges of the screen
		if (ball_x <= 0)
		{
			delta_x = +2;
			if (delta_y == 0 && rand() % 3 == 0) delta_y = +1;
		} 
		else if (ball_x >= SCREEN_WIDTH - ball->width) 
		{
			delta_x = -2;			
			if (delta_y == 0 && rand() % 3 == 0) delta_y = +1;
		}

		// if ball goes in a straight line, randomize its bouncing
		if (ball_y <= 0) 
		{
			if (delta_x == 0 && rand() % 3 == 0) delta_x = +1;
			delta_y = +1;
		}
		else if (ball_y >= SCREEN_HEIGHT - ball->height) 
		{
			if (delta_x == 0 && rand() % 3 == 0) delta_x = +1;
			delta_y = -1;			
		}

		ball_x += delta_x;
		ball_y += delta_y;

		// refresh the screen
		draw_background(spriteFactory->get(GREY_PATTERN));
		ball->draw(ball_x, ball_y);
		Bdisp_PutDisp_DD();	

		// and wait
		Sleep(REFRESH_FREQUENCY);		
    }

    return 1;
}

