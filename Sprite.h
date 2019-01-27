/*
 * All sprite-related classes: Sprite and SpriteFactory.
 * A sprite is a bitmap, with an optional mask. It knows how to draw itself to the VRAM.
 * The sprite factory loads sprites into main memory.
 */

enum SpriteKind
{
	NULL_SPRITE,
	BLACK_PATTERN,
	LIGHT_PATTERN,
	MEDIUM_PATTERN,
	DARK_PATTERN,
	ZEBRA_PATTERN,
	WHITE_BALL,
	BLACK_BALL
};

class Sprite 
{
	public:
		const SpriteKind kind;
		const unsigned int width;
		const unsigned int height;
		const unsigned char *mask;
		const unsigned char *bitmap;	

		Sprite(SpriteKind k, int w, int h, const unsigned char *bm, const unsigned char *m = 0)
			: kind(k), width(w), height(h), bitmap(bm), mask(m)
		{ }

		void draw(int x, int y) const;
};

class SpriteFactory
{
	public:
		SpriteFactory();
		void load_all();
		const Sprite* get(SpriteKind sk);
		const Sprite* next_pattern(const Sprite* pattern);		

	private:
		const Sprite* sprites[99];	// max number of sprites
};

