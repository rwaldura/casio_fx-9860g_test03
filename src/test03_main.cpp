/*
 * Game for Casio fx-9860G
 * by Ren Waldura, January 2019
 */

#include <stdlib.h>

#include "Sprite.h"
#include "Game.h"

extern "C"
{
	#include "fxlib.h"
}

/*
 * Heartbeat of this program. We compute changes, and refresh the display
 * accordingly, every REFRESH_FREQUENCY ms.
 */
const int REFRESH_FREQUENCY = 100; // ms

/*
 * The Casio fx-9860G has a simple LCD monochrome display.
 * Each pixel is one bit, where 0 = off = white
 * And bit 1 = on = black.
 */
const int DISPLAY_WIDTH = IM_VRAM_WIDTH; 
const int DISPLAY_HEIGHT = IM_VRAM_HEIGHT; // pixels i.e. bits

enum Action
{
	NULL_ACTION,
	EXIT_ACTION, 
	MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
	CHANGE_BACKGROUND, CHANGE_BALL
};

void draw_background(const Sprite* pattern)
{	
	if (pattern->kind == NULL_SPRITE)
	{
		Bdisp_AllClr_VRAM();
	}
	else
	{
		for (int x = 0; x < DISPLAY_WIDTH; x += pattern->width)
		{
			for (int y = 0; y < DISPLAY_HEIGHT; y += pattern->height)
			{
				pattern->draw(x, y);
			}
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

	if (IsKeyDown(KEY_CHAR_PLUS))
		a = CHANGE_BACKGROUND;
	else if (IsKeyDown(KEY_CHAR_MINUS))
		a = CHANGE_BALL;

	return a;
}

static void log(const unsigned char* mesg)
{
	static int line = 0;
	PrintMini(0, line, mesg, MINI_OVER);
	line += 6;
	unsigned int key; GetKey(&key); // blocks until key pressed
}

/*
 * Main function and game loop.
 */
extern "C" int test03_main(int isAppli, unsigned short optionNum)
{
	// load all the sprites, we're going to need them
	SpriteManager sm;
	sm.load_all();

	// the background 
	const Sprite* background_pattern = sm.get(NULL_SPRITE);

	// the paddle 
	GameObject* paddle = new GameObject(16, 6, sm.get(MINI_PADDLE));
	paddle->move_to(22, DISPLAY_HEIGHT - paddle->height - 2);

	// the ball 
	GameObject* ball = new GameObject(6, 6, sm.get(MINI_BALL));
	ball->move_to(1, 1);
	ball->delta_x = 4;
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
				// no-op
				break;

			case MOVE_DOWN:
				// no-op
				break;

			case MOVE_LEFT:
				paddle->x -= 10;
				break;

			case MOVE_RIGHT:
				paddle->x += 10;
				break;

			// cycle the background
			case CHANGE_BACKGROUND:
				background_pattern = sm.next_pattern(background_pattern);
				break;

			// rotate the ball
			case CHANGE_BALL:
				ball->sprite = sm.next_ball(ball->sprite);
				break;
		}

		// CONSTRAINT: ball bounces off paddle
		if (ball->intersects(paddle))
		{
			ball->delta_x = -ball->delta_x;
			ball->delta_y = -ball->delta_y;
		}

		// CONSTRAINT: paddle must stay in play area
		if (paddle->x <= 1) paddle->x = 1;
		if (paddle->x >= DISPLAY_WIDTH - paddle->width) paddle->x = DISPLAY_WIDTH - paddle->width;

		// CONSTRAINT: ball bounces off the edges of the screen
		// if ball goes in a straight line, randomize its bouncing
		if (ball->x <= 0)
		{
			ball->delta_x = +3;
			if (ball->delta_y == 0 && rand() % 3 == 0) ball->delta_y = +1;
		} 
		else if (ball->x >= DISPLAY_WIDTH - ball->width) 
		{
			ball->delta_x = -3;			
			if (ball->delta_y == 0 && rand() % 3 == 0) ball->delta_y = +1;
		}

		if (ball->y <= 0) 
		{
			if (ball->delta_x == 0 && rand() % 3 == 0) ball->delta_x = +1;
			ball->delta_y = +1;
		}
		else if (ball->y >= DISPLAY_HEIGHT - ball->height) 
		{
			if (ball->delta_x == 0 && rand() % 3 == 0) ball->delta_x = +1;
			ball->delta_y = -1;			
		}

		ball->move();

		// refresh the screen
		draw_background(background_pattern);
		ball->draw();
		paddle->draw();
		Bdisp_PutDisp_DD();	

		// and wait
		Sleep(REFRESH_FREQUENCY);		
    }

    return 1; // NO_ERROR
}

