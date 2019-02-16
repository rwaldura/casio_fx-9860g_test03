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
#include "GetAppName.h"

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
 * Hack
 */
bool isMainMenuRunning()
{
	return true;
}

/*
 * Update the game and refresh the display.
 * 
 * Timers are not suspended when the main menu gets called from GetKey().
 * However, we prevent overtaking the display when we know we're not in
 * the "foreground".
 */
void refresh(void)
{
	if (game && !isMainMenuRunning())
	{
		game->update();	

		// refresh the display from the VRAM
		Bdisp_PutDisp_DD();	
	}

	unsigned char appName[11];
	GetAppName(appName);
	appName[10] = '\0';
	PrintMini(1, 1, appName, MINI_OVER);
}

/*
 * Main function and game loop.
 */
extern "C" int test03_main(int isAppli, unsigned short optionNum)
{
	game = new Game(IM_VRAM_WIDTH, IM_VRAM_HEIGHT);
	// load all sprites etc.
	game->start();

	Bkey_Set_RepeatTime(REFRESH_FREQ / 25, REFRESH_FREQ / 25);
	SetTimer(ID_USER_TIMER1, REFRESH_FREQ, refresh);
	SetQuitHandler(finish);

	/*
 	 * This is apparently the proper way to write a Casio add-in: they
	 * are not meant to return, ever.
	 * Instead, GetKey() should be called regularly to let the OS handle
	 * things, particularly the MENU key, used to exit add-ins. 
	 * An add-in that exits -- returns from main() -- cannot be resumed from
	 * the main menu.
 	 */
	for ( ; /* ever */ ; )
    {
		// GetKey() blocks waiting for a keypress
		// also refreshes the display, same as Bdisp_PutDisp_DD()
		// also handles MENU key and contrast setting
		unsigned int key;
		GetKey(&key);
		
		game->handle_event(key);

		// game->update();
		// this is done by our timer
    }	             

    return 1; // NOTREACHED
}
