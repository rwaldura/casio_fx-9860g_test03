/*
 * All sprite-related classes: Sprite and SpriteManager.
 * A sprite is a bitmap, with an optional mask. It knows how to draw itself to the VRAM.
 * The sprite manager loads sprites into main memory.
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "FileReader.h"

enum SpriteKind
{
	NULL_SPRITE,
	BLACK_PATTERN,
	LIGHT_PATTERN,
	MEDIUM_PATTERN,
	DARK_PATTERN,
	ZEBRA_PATTERN,
	MINI_BALL,
	WHITE_BALL,
	BLACK_BALL,
	MINI_PADDLE
};

class Sprite 
{
	public:
		const SpriteKind kind;
		const unsigned int width;
		const unsigned int height;

		// these are made public for unit-testing purposes
		// they should be private otherwise
		const unsigned char *const bitmap;	
		const unsigned char *const mask;

		Sprite(SpriteKind k, int w, int h, const unsigned char *bm, const unsigned char *m = 0)
			: kind(k), width(w), height(h), bitmap(bm), mask(m)
		{}

		~Sprite()
		{
			delete mask;
			delete bitmap;
		}

		// draw the sprite into VRAM
		void draw(int x, int y) const;
};

/*
 * Keeper of the sprites. Loads them into memory.
 */
class SpriteManager
{
	public:
		SpriteManager() : loaded_from_file(false) 
		{}

		~SpriteManager()
		{
			if (loaded_from_file)
				for (int i = 0; i < MAX_SPRITES; i++)
					delete sprites[i];
		}

		const Sprite* get(SpriteKind sk)
		{
			return sprites[sk];			
		}

		const Sprite* next_pattern(const Sprite* s);		
		const Sprite* next_ball(const Sprite* s);		

		int load_file(const char* filename); // 0 => no error
		void load_all();

	private:
		static const int MAX_SPRITES = 32;
		const Sprite* sprites[MAX_SPRITES];
		bool loaded_from_file;
};

/*
 * Reads the sprites from file, and instantiate them.
 */
class SpriteBuilder
{
	public:
		SpriteBuilder(FileReader& r) : reader(r) 
		{}

		const Sprite* build_sprite();

		bool is_done()
		{
			return reader.at_end();
		}

		unsigned char* parse_bitmap_string(int width, int height, const char* s);
		const Sprite* _build_sprite(int id, int width, int height, const char* bitmap_str);

	private:
		FileReader& reader;
};

#endif
