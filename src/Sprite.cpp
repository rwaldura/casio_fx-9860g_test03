/*
 * All sprite-related classes: Sprite and SpriteFactory.
 * A sprite is a bitmap, with an optional mask. It knows how to draw itself to the VRAM.
 *
 * The sprite factory loads sprites into main memory.
 * I wish I could read sprites from a file; I don't know how to do that.
 * Instead, sprites are defined inline with byte arrays.
 */

#include <stdlib.h>
#include <string.h>
#include "Sprite.h"
#include "FileReader.h"

extern "C"
{
	#include "fxlib.h"
}

static const unsigned char light_pattern_bmap[] = {
    0x00, 0x22, 0x44, 0x00, 0x00, 0x22, 0x44, 0x00
};

static const unsigned char medium_pattern_bmap[] = {
	0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 
};

static const unsigned char dark_pattern_bmap[] = {
	0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 
};

static const unsigned char zebra_pattern_bmap[] = {
    0x38, 0x71, 0xe3, 0xc7, 0x8e, 0x1c, 0x38, 0x70
};

static const unsigned char black_pattern_bmap[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const unsigned char black_ball_sprite_mask[] = {
	0xe7, 0x81, 0x81, 0x00, 0x00, 0x81, 0x81, 0xe7
};
static const unsigned char black_ball_sprite_bmap[] = {
	0x00, 0x18, 0x3c, 0x76, 0x7e, 0x3c, 0x18, 0x00
};

static const unsigned char white_ball_sprite_mask[] = {
    0xe7, 0xc3, 0x81, 0x00, 0x00, 0x81, 0xc3, 0xe7
};
static const unsigned char white_ball_sprite_bmap[] = {
    0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00
};

static const unsigned char mini_ball_sprite_bmap[] = {
    0x00, 0x00, 0x08, 0x14, 0x08, 0x00, 0x00, 0x00
};

static const unsigned char mini_ball_sprite_mask[] = {
    0xff, 0xe3, 0xc1, 0xc1, 0xc1, 0xe3, 0xff, 0xff
};

/* sprites/paddle-mini.bmp bitmap 16x16 */
static const unsigned char mini_paddle_bitmap[] = {
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x30, 0x0c, 
	0x55, 0x56, 
	0x6a, 0xaa, 
	0x30, 0x0c, 
	0x00, 0x00, 
	0x00, 0x00
};
/* sprites/paddle-mini.bmp mask 16x16 */
static const unsigned char mini_paddle_mask[] = {
	0xff, 0xff, 
	0xff, 0xff, 
	0xff, 0xff, 
	0xff, 0xff, 
	0xff, 0xff, 
	0xff, 0xff, 
	0xff, 0xff, 
	0xff, 0xff, 
	0xff, 0xff, 
	0xcf, 0xf3, 
	0x80, 0x01, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x80, 0x01, 
	0xcf, 0xf3, 
	0xff, 0xff
};

static const int MAX_BITMAP_SIZE = IM_VRAM_SIZE; // in bytes

static unsigned char* parse_bitmap_string(int width, int height, const char* s)
{
	unsigned char* bitmap = new unsigned char[width * height / 8](); // zero-initialized array

	for (int i = 0; i < width * height; i++)
	{
		// lit black pixel is marked by '#' character
		if (s[i] == '#')
		{
			bitmap[i / 8] |= 1 << (7 - i % 8);			
		}
	}

	return bitmap;
}

static const Sprite* read_sprite(FileReader* r)
{
	int id = 0;
	int width = 0, height = 0;
	char* bitmap_str = new char[MAX_BITMAP_SIZE];
	bool end = false;

	// read one sprite definition from the file
	char* line;
	while (!end && (line = r->read_line()))
	{
		// skip comments: lines that start with "//"
		if (strncmp(line, "//", 2) == 0) 
		{
			// skip
		}
		// read the sprite ID
		else if (strncmp(line, "ID=", 3) == 0)
		{
			id = atoi(line + 3);
		}
		// blank line signals end of sprite
		else if (strlen(line) == 0)
		{
			end = true;
		}
		// read bitmap
		else
		{
			if (width == 0) width = strlen(line);
			strcat(bitmap_str, line);
			height += 1;
		}

		delete line;
	}

	// invalid bitmap definition
	if (!(strlen(bitmap_str) == width * height 
		|| strlen(bitmap_str) == width * height * 2))
		return 0;

	// parse the bitmap definition into actual bits
  	unsigned char* bitmap = parse_bitmap_string(width, height, bitmap_str);
	
	unsigned char* mask = 0;
	if (strlen(bitmap_str) == width * height * 2)
	{
		mask = parse_bitmap_string(width, height, bitmap_str + width * height);
	}
	
	delete bitmap_str;

	return new Sprite((SpriteKind) id, width, height, bitmap, mask);	
}

int SpriteManager::load_file(const char* filename)
{
	FileReader* r = new FileReader();
	int err = r->open(filename);
	if (err) return err;

	while (!r->at_end())
	{
		const Sprite* s = read_sprite(r);

		if (s)
			sprites[s->kind] = s;
	}

	return 0; // no error
}

void SpriteManager::load_all()
{
	sprites[NULL_SPRITE] = new Sprite(NULL_SPRITE, 0, 0, 0);

	sprites[LIGHT_PATTERN] = new Sprite(LIGHT_PATTERN, 8, 8, light_pattern_bmap);
	sprites[MEDIUM_PATTERN] = new Sprite(MEDIUM_PATTERN, 8, 8, medium_pattern_bmap);
	sprites[DARK_PATTERN] = new Sprite(DARK_PATTERN, 8, 8, dark_pattern_bmap);
	sprites[BLACK_PATTERN] = new Sprite(BLACK_PATTERN, 8, 8, black_pattern_bmap);
	sprites[ZEBRA_PATTERN] = new Sprite(ZEBRA_PATTERN, 8, 8, zebra_pattern_bmap);

	sprites[MINI_BALL] = new Sprite(MINI_BALL, 8, 8, mini_ball_sprite_bmap, mini_ball_sprite_mask);
	sprites[WHITE_BALL] = new Sprite(WHITE_BALL, 8, 8, white_ball_sprite_bmap, white_ball_sprite_mask);
	sprites[BLACK_BALL] = new Sprite(BLACK_BALL, 8, 8, black_ball_sprite_bmap, black_ball_sprite_mask);

	sprites[MINI_PADDLE] = new Sprite(MINI_PADDLE, 16, 16, mini_paddle_bitmap, mini_paddle_mask);
} 

const Sprite* SpriteManager::next_pattern(const Sprite* pattern)		
{
	// ZEBRA_PATTERN is the last pattern
	SpriteKind next_kind = (SpriteKind) (1 + (pattern->kind + 1) % ZEBRA_PATTERN);
	return this->get(next_kind);
}

const Sprite* SpriteManager::next_ball(const Sprite* ball)		
{
	SpriteKind next_kind;
	if (ball->kind == MINI_BALL)
		next_kind = WHITE_BALL;
	else if (ball->kind == WHITE_BALL)
		next_kind = BLACK_BALL;
	else if (ball->kind == BLACK_BALL)
		next_kind = MINI_BALL;

	return this->get(next_kind);
}

/* 
 * Draw a sprite to VRAM: punch a hole in the bitmap by ANDing the mask first,
 * then insert the actual sprite bitmap.
 */
void Sprite::draw(int x, int y) const
{
	DISPGRAPH dg = { x, y };
	dg.GraphData.width = width;
	dg.GraphData.height = height;
	dg.WriteModify = IMB_WRITEMODIFY_NORMAL;

	if (mask)
	{
		// mask first to clear the area
		dg.GraphData.pBitmap = (unsigned char*) mask;
		dg.WriteKind = IMB_WRITEKIND_AND;
		Bdisp_WriteGraph_VRAM(&dg);		

		// the actual sprite
		dg.GraphData.pBitmap = (unsigned char*) bitmap;
		dg.WriteKind = IMB_WRITEKIND_OR;
		Bdisp_WriteGraph_VRAM(&dg);
	}
	else // no mask: merely fill
	{
		dg.GraphData.pBitmap = (unsigned char*) bitmap;
		dg.WriteKind = IMB_WRITEKIND_OVER;
		Bdisp_WriteGraph_VRAM(&dg);		
	}
}
