/*
 * Game for Casio fx-9860G
 * by Ren Waldura, January 2019
 */

#include <stdlib.h>
#include "Sprite.h"

extern "C"
{
	#include "fxlib.h"
}

/*
 * Heartbeat of this program. We compute changes, and refresh the display
 * accordingly, every REFRESH_FREQUENCY ms.
 */
const int REFRESH_FREQUENCY = 200; // ms

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
	for (int x = 0; x < DISPLAY_WIDTH; x += pattern->width)
	{
		for (int y = 0; y < DISPLAY_HEIGHT; y += pattern->height)
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
	unsigned int key; GetKey(&key);
}

/*
 * An object in the game: the ball, the paddle, the bricks.
 * The size of an object can differ from the size of its sprite! 
 * This lets us accommodate drop shadows etc. around the object itself.
 */
class GameObject
{
public:
	const Sprite* sprite;
	const int width, height;
	int x, y; // location in the game area
	int delta_x, delta_y;

	GameObject(int h, int w, const Sprite* s = 0, int xx = 0, int yy = 0) 
		: width(w), height(h), sprite(s), x(xx), y(yy) 
	{
	}

	void draw() const
	{
		this->sprite->draw(this->x, this->y);		
	}
};

/*
 * Main function and game loop.
 */
extern "C" int test03_main(int isAppli, unsigned short optionNum)
{
	// load all the sprites, we're going to need them
	SpriteFactory* factory = new SpriteFactory();
	factory->load_all();

	// the background 
	const Sprite* background_pattern = factory->get(BLACK_PATTERN);

	// the ball 
	GameObject* ball = new GameObject(6, 6, factory->get(WHITE_BALL));
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

			// rotate the background
			case CHANGE_BACKGROUND:
				background_pattern = factory->next_pattern(background_pattern);
				break;

			// rotate the ball
			case CHANGE_BALL:
				ball->sprite = (ball->sprite->kind == WHITE_BALL) 
					? factory->get(BLACK_BALL) 
					: factory->get(WHITE_BALL);
				break;
		}

		// ball bounces off the edges of the screen
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

		// if ball goes in a straight line, randomize its bouncing
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

		ball->x += ball->delta_x;
		ball->y += ball->delta_y;

		// refresh the screen
		draw_background(background_pattern);
		ball->draw();
		Bdisp_PutDisp_DD();	

		// and wait
		Sleep(REFRESH_FREQUENCY);		
    }

    return 1;
}

