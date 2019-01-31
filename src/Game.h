/*
 * An object in the game: the ball, the paddle, the bricks.
 * The size of an object can differ from the size of its sprite! 
 * This lets us accommodate drop shadows etc. around the object itself.
 */

#include "Sprite.h"

class GameObject
{
public:
	const Sprite* sprite;	
	const int width, height;
	int x, y; // location in the game area
	int delta_x, delta_y;

	GameObject(int w, int h, const Sprite* s, int xx = 0, int yy = 0) 
		: width(w), height(h), sprite(s), x(xx), y(yy) 
	{
	}

	void draw() const
	{
		sprite->draw(x - sprite->width + width, y - sprite->height + height);		
	}

	void move()
	{
		x += delta_x;
		y += delta_y;
	}

	void move_to(int xx, int yy)
	{
		x = xx;
		y = yy;
	}

	bool intersects(const GameObject* o) const
	{
		if (   x +    width  < o->x 
		 || o->x + o->width  <    x 
		 ||    y +    height < o->y 
		 || o->y + o->height <    y)
			return false;
		else
		    return true;
	}
};
