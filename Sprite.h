class Sprite 
{
public:
	const unsigned int width;
	const unsigned int height;
	const unsigned char *mask;
	const unsigned char *bitmap;	

	Sprite(int w, int h, unsigned char *bm, unsigned char *m = 0)
		: width(w), height(h), bitmap(bm), mask(m)
	{ }
};

enum SpriteKind
{
	NULL_SPRITE,
	WHITE_BALL,
	BLACK_BALL,
	GREY_PATTERN,
	ZEBRA_PATTERN
};

class SpriteFactory
{
public:
	SpriteFactory();
	void load_all();
	const Sprite* get(SpriteKind sk);
private:
	const Sprite* sprites[12]; 
};

