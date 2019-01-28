/*
 * All sprite-related classes: Sprite and SpriteFactory.
 * A sprite is a bitmap, with an optional mask. It knows how to draw itself to the VRAM.
 *
 * The sprite factory loads sprites into main memory.
 * I wish I could read sprites from a file; I don't know how to do that.
 * Instead, sprites are defined inline with byte arrays.
 */

extern "C"
{
	#include "fxlib.h"
}

#include "Sprite.h"


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

static const unsigned char simple_paddle_bitmap[] = {
};
static const unsigned char simple_paddle_mask[] = {
};

SpriteFactory::SpriteFactory()
{
	;
}

SpriteFactory::~SpriteFactory()
{
	for (int i = 0; i < this->MAX_SPRITES; i++)
	{
		if (sprites[i]) delete sprites[i];
	}
}

void SpriteFactory::load_all()
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

	sprites[SIMPLE_PADDLE] = new Sprite(SIMPLE_PADDLE, 16, 16, simple_paddle_bitmap, simple_paddle_mask);
} 

const Sprite* SpriteFactory::get(SpriteKind sk)
{
	return sprites[sk];
}

const Sprite* SpriteFactory::next_pattern(const Sprite* pattern)		
{
	// ZEBRA_PATTERN is the last pattern
	SpriteKind next_kind = (SpriteKind) (1 + (pattern->kind + 1) % ZEBRA_PATTERN);
	return this->get(next_kind);
}

const Sprite* SpriteFactory::next_ball(const Sprite* ball)		
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
	dg.WriteModify = IMB_WRITEMODIFY_NORMAL;
	dg.GraphData.width = width;
	dg.GraphData.height = height;

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
	else
	{
		// no mask: merely fill
		dg.GraphData.pBitmap = (unsigned char*) bitmap;
		dg.WriteKind = IMB_WRITEKIND_OVER;
		Bdisp_WriteGraph_VRAM(&dg);		
	}
}
