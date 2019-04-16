# casio_fx-9860g_test03
Game programming on the Casio fx-9860G graphing calculator

This is merely a test program, a game, for the Casio fx-9860 SDK.
Code is written in C++; calls are made to the Casio primitives defined in fxlib.h.

# Where I'm stuck
I have identified 2 options to code an interactive game in Casio: block on `sleep()`, or wait for events.

## Block on sleep()
It looks like this:

```C
while (true)
{
	// peek at keyboard state: if some keys are pressed, do something
	doSomethingWhenKeysPressed();
	
	refreshDisplay();
	
	// then wait for a small amount of time 
	sleep(SOME_SMALL_AMOUNT_OF_TIME);
}
```

Pro: easy to understand.

Cons:
* Does not support the Menu key, or other OS-level functions like changing the contrast.
* The value for `SOME_SMALL_AMOUNT_OF_TIME` dictates game liveliness.
* Casio's `sleep()` is actually implemented as a busy loop, not a real sleep. Blech.

## Wait for Events
```C
while (keyPressed = GetKey())
{
	doSomethingForKeysPressed(keyPressed);
}
```

Pros:
* handles Menu key and standard functions
* correctly blocks waiting for a keyboard event

Cons:
* Nothing happens outside of events. How to run an animation? The basis of an interactive game. 
* To do so, I install a user timer, to refresh the display at regular intervals.
* Said timer is not suspended when the Main Menu is called up, by user pressing the Menu key
* Timer function draws all over Main Menu, makes it unusable

Where I'm stuck: I don't know how to prevent this; I don't know how to suspend a timer, when Main Menu is called.
Or, alternatively, detect Main Menu is running, and avoid executing the timer.

Source file is [test03_main.cpp](tree/master/src/test03_main.cpp).

See also [Tutoriel Gestion du Clavier](https://www.planet-casio.com/Fr/forums/topic14175-5-tutoriel-la-gestion-du-clavier-en-c.html).
A solution is provided, but I find it unsatisfying.
