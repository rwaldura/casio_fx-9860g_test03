/*
 * Game for Casio fx-9860G
 *
 * The Casio fx-9860G has a simple LCD monochrome display.
 * Each pixel is one bit, where 0 = off = white
 * And bit 1 = on = black.
 *
 * by Ren Waldura ren+casio@waldura.org
 * January 2019
 */

#include "Game.h"

extern "C"
{
	#include "timer.h"
	#include "fxlib.h"
}

/*
 * Heartbeat of this program. We compute changes, and refresh the display
 * accordingly, every REFRESH_FREQ ms.
 */
static const int REFRESH_FREQ = 100; // ms

Game* game = 0;

void finish(void)
{
	Bkey_Set_RepeatTime_Default();

	KillTimer(ID_USER_TIMER1);

	if (game)
	{
		game->end();
		delete game;
		game = 0;
	}
}

/*
 * Update the game and refresh the display.
 */
void refresh(void)
{
	if (game)
	{
		game->update();		
		Bdisp_PutDisp_DD();	
	}
}

/*
 * Main function and game loop.
 */
extern "C" int test03_main(int isAppli, unsigned short optionNum)
{
	SetQuitHandler(finish);

	Bkey_Set_RepeatTime(REFRESH_FREQ / 25, REFRESH_FREQ / 25);

	SetTimer(ID_USER_TIMER1, REFRESH_FREQ, refresh);

	game = new Game(IM_VRAM_WIDTH, IM_VRAM_HEIGHT);
	game->start();
             
	bool quit = false;
	while (!quit)
    {
		// GetKey() blocks waiting for a keypress
		// also refreshes the display
		// also handles MENU key and contrast setting
		unsigned int key;
		GetKey(&key);
		
		quit = game->handle_event(key);
		// game->update();
    }	             

	finish();

    return 1; // NO_ERROR
}
