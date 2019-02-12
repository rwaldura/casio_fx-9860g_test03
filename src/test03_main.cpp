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
 * accordingly, every REFRESH_FREQUENCY ms.
 */
static const int REFRESH_FREQUENCY = 500; // ms

Game* game = 0;

void end_game(void)
{
	KillTimer(ID_USER_TIMER1);

	if (game)
	{
		game->end();
		delete game;
		game = 0;
	}
}

void update_game(void)
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
	SetQuitHandler(end_game);
	SetTimer(ID_USER_TIMER1, REFRESH_FREQUENCY, update_game);

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
		game->update();
    }	             

	end_game();

    return 1; // NO_ERROR
}
