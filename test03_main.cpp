/*
 * ARKANOID 
 * by Ren Waldura ren+fx9860@waldura.org
 * License: 
 *
 * THINGS TO KEEP IN MIND
 * bit 0 = off = white
 * bit 1 = on = black
 */

extern "C"
{
#include "stdlib.h"
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

struct Sprite 
{                                                     
	unsigned int width;
	unsigned int height;
	unsigned char *mask;
	unsigned char *bitmap;
};

const Sprite NULL_SPRITE = { 0, 0, 0, 0 };

unsigned char grey_pattern_bmap[] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
const Sprite GREY_PATTERN = { 8, 8, 0, grey_pattern_bmap };

unsigned char zebra_pattern_bmap[] = { 0x92, 0x49, 0x24, 0x92, 0x49, 0x24, 0x92, 0x49 };
const Sprite ZEBRA_PATTERN = { 8, 8, 0, zebra_pattern_bmap };

unsigned char ball_sprite_mask[] = {
    0xe7, 0x81, 0x81, 0x00, 0x00, 0x81, 0x81, 0xe7
};
unsigned char ball_sprite_bmap[] = {
    0x00, 0x18, 0x3c, 0x76, 0x7e, 0x3c, 0x18, 0x00
};                                                   
const Sprite BALL_SPRITE = { 8, 8, ball_sprite_mask, ball_sprite_bmap };

void draw_sprite(const Sprite& sprite, int x, int y)
{
	DISPGRAPH dispGraph;
	dispGraph.x = x;
	dispGraph.y = y;
	dispGraph.WriteModify = IMB_WRITEMODIFY_NORMAL;
	dispGraph.GraphData.width = sprite.width;
	dispGraph.GraphData.height = sprite.height;

	/* draw mask */
	if (sprite.mask)
	{
		dispGraph.GraphData.pBitmap = sprite.mask;
		dispGraph.WriteKind = IMB_WRITEKIND_AND;
		Bdisp_WriteGraph_VRAM(&dispGraph);		
	}

	/* draw bitmap */
	dispGraph.GraphData.pBitmap = sprite.bitmap;
	dispGraph.WriteKind = IMB_WRITEKIND_OR;
	Bdisp_WriteGraph_VRAM(&dispGraph);
}

void draw_background(const Sprite& pattern)
{
    Bdisp_AllClr_VRAM();

	for (int x = 0; x < SCREEN_WIDTH; x += pattern.width)
	{
		for (int y = 0; y < SCREEN_HEIGHT; y += pattern.height)
		{
			draw_sprite(pattern, x, y);
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

extern "C" int test03_main(int isAppli, unsigned short optionNum)
{
	int ball_x = SCREEN_WIDTH / 2;
	int ball_y = SCREEN_HEIGHT / 2;
	int delta_x = 2;
	int delta_y = 1;

	int quit = 0;
	while (!quit)
    {
		Action action = handle_keyboard();
		switch (action)
		{
			case EXIT_ACTION:
				quit = 1;
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
		else if (ball_x >= SCREEN_WIDTH - BALL_SPRITE.width) 
		{
			delta_x = -2;			
			if (delta_y == 0 && rand() % 3 == 0) delta_y = +1;
		}

		if (ball_y <= 0) 
		{
			if (delta_x == 0 && rand() % 3 == 0) delta_x = +1;
			delta_y = +1;
		}
		else if (ball_y >= SCREEN_HEIGHT - BALL_SPRITE.height) 
		{
			if (delta_x == 0 && rand() % 3 == 0) delta_x = +1;
			delta_y = -1;			
		}

		ball_x += delta_x;
		ball_y += delta_y;

		// refresh the screen
		draw_background(GREY_PATTERN);
		draw_sprite(BALL_SPRITE, ball_x, ball_y);
		Bdisp_PutDisp_DD();	

		// and wait
		Sleep(REFRESH_FREQUENCY);		
    }

    return 1;
}

