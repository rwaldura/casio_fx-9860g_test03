#include <stdlib.h>

#include "Game.h"

extern "C"
{
	#include "fxlib.h"
}

void Game::start()
{
	// load all the sprites, we're going to need them
	sm = new SpriteManager();
	sm->load_all();

	// the background 
	background_pattern = sm->get(NULL_SPRITE);

	// the paddle 
	paddle = new GameObject(16, 6, sm->get(MINI_PADDLE));
	paddle->move_to(22, max_height - paddle->height - 2);

	// the ball 
	ball = new GameObject(6, 6, sm->get(MINI_BALL));
	ball->move_to(1, 1);
	ball->delta_x = 4;
	ball->delta_y = 2;	
}

void Game::end()
{
	if (paddle)
	{
		delete paddle;
		paddle = 0;		
	}

	if (ball)
	{
		delete ball;
		ball = 0;		
	}

	if (sm)
	{
		delete sm; // delete all sprites
		sm = 0;		
	}
}

void Game::draw_background(const Sprite* pattern)
{	
	if (pattern->kind == NULL_SPRITE)
	{
		Bdisp_AllClr_VRAM();
	}
	else
	{
		for (int y = 0; y < max_height; y += pattern->height)
		{
			for (int x = 0; x < max_width; x += pattern->width)
			{
				pattern->draw(x, y);
			}
		}		
	}
}

bool Game::handle_event(unsigned int key)
{
	switch (key)
	{
		case KEY_CTRL_EXIT:
			return true;
		
		case KEY_CTRL_UP:
			// no-op
			break;

		case KEY_CTRL_DOWN:
			// no-op
			break;

		case KEY_CTRL_LEFT:
			paddle->x -= 10;
			break;

		case KEY_CTRL_RIGHT:
			paddle->x += 10;
			break;

		// cycle the background
		case KEY_CHAR_PLUS:
			background_pattern = sm->next_pattern(background_pattern);
			break;

		// rotate the ball
		case KEY_CHAR_MINUS:
			ball->sprite = sm->next_ball(ball->sprite);
			break;
	}

	return false;
}

static void log(const unsigned char* mesg)
{
	static int line = 0;
	PrintMini(0, line, mesg, MINI_OVER);
	line += 6;
}

/*
 * Compute game changes, and update the VRAM.
 */
void Game::update()
{
	// CONSTRAINT: ball bounces off paddle
	if (ball->intersects(*paddle))
	{
		ball->delta_x = -ball->delta_x;
		ball->delta_y = -ball->delta_y;
	}

	// CONSTRAINT: paddle must stay in play area
	if (paddle->x <= 1) paddle->x = 1;
	if (paddle->x >= max_width - paddle->width) paddle->x = max_width - paddle->width;

	// CONSTRAINT: ball bounces off the edges of the screen
	// if ball goes in a straight line, randomize its bouncing
	if (ball->x <= 0)
	{
		ball->delta_x = +3;
		if (ball->delta_y == 0 && rand() % 3 == 0) ball->delta_y = +1;
	} 
	else if (ball->x >= max_width - ball->width) 
	{
		ball->delta_x = -3;			
		if (ball->delta_y == 0 && rand() % 3 == 0) ball->delta_y = +1;
	}

	if (ball->y <= 0) 
	{
		if (ball->delta_x == 0 && rand() % 3 == 0) ball->delta_x = +1;
		ball->delta_y = +1;
	}
	else if (ball->y >= max_height - ball->height) 
	{
		if (ball->delta_x == 0 && rand() % 3 == 0) ball->delta_x = +1;
		ball->delta_y = -1;			
	}

	ball->move();

	// refresh the screen
	draw_background(background_pattern);
	ball->draw();
	paddle->draw();
}
