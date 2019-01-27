/*
 * All sprite-related classes: Sprite and SpriteFactory.
 * A sprite is a bitmap, with an optional mask. It knows how to draw itself to the VRAM.
 *
 * The sprite factory loads sprites into main memory.
 * I wish I could read sprites from a file; I don't know how to do that.
 * Instead, sprites are defined inline with byte arrays.
 */

#include "Sprite.h"

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

SpriteFactory::SpriteFactory()
{
	;
}

void SpriteFactory::load_all()
{
	sprites[NULL_SPRITE] = new Sprite(NULL_SPRITE, 0, 0, 0);

	sprites[LIGHT_PATTERN] = new Sprite(LIGHT_PATTERN, 8, 8, light_pattern_bmap);
	sprites[MEDIUM_PATTERN] = new Sprite(MEDIUM_PATTERN, 8, 8, medium_pattern_bmap);
	sprites[DARK_PATTERN] = new Sprite(DARK_PATTERN, 8, 8, dark_pattern_bmap);
	sprites[BLACK_PATTERN] = new Sprite(BLACK_PATTERN, 8, 8, black_pattern_bmap);
	sprites[ZEBRA_PATTERN] = new Sprite(ZEBRA_PATTERN, 8, 8, zebra_pattern_bmap);

	sprites[WHITE_BALL] = new Sprite(WHITE_BALL, 8, 8, white_ball_sprite_bmap, white_ball_sprite_mask);
	sprites[BLACK_BALL] = new Sprite(BLACK_BALL, 8, 8, black_ball_sprite_bmap, black_ball_sprite_mask);
} 

const Sprite* SpriteFactory::get(SpriteKind sk)
{
	return sprites[sk];
}

const Sprite* SpriteFactory::next_pattern(const Sprite* pattern)		
{
	SpriteKind next_kind = (SpriteKind) (1 + (pattern->kind + 1) % ZEBRA_PATTERN);
	return this->get(next_kind);
}

/* 
 * Draw a sprite to VRAM: punch a hole in the bitmap by ANDing the mask first,
 * then insert the actual sprite bitmap.
 */
void Sprite::draw(int x, int y) const
{
	DISPGRAPH dispGraph;
	dispGraph.x = x;
	dispGraph.y = y;
	dispGraph.WriteModify = IMB_WRITEMODIFY_NORMAL;
	dispGraph.GraphData.width = this->width;
	dispGraph.GraphData.height = this->height;

	if (this->mask)
	{
		// mask first to clear the area
		dispGraph.GraphData.pBitmap = (unsigned char*) this->mask;
		dispGraph.WriteKind = IMB_WRITEKIND_AND;
		Bdisp_WriteGraph_VRAM(&dispGraph);		

		// the actual sprite
		dispGraph.GraphData.pBitmap = (unsigned char*) this->bitmap;
		dispGraph.WriteKind = IMB_WRITEKIND_OR;
		Bdisp_WriteGraph_VRAM(&dispGraph);
	}
	else
	{
		// no mask: merely fill
		dispGraph.GraphData.pBitmap = (unsigned char*) this->bitmap;
		dispGraph.WriteKind = IMB_WRITEKIND_OVER;
		Bdisp_WriteGraph_VRAM(&dispGraph);		
	}
}






