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

static unsigned char grey_pattern_bmap[] = {
	0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 
};

static unsigned char zebra_pattern_bmap[] = {
	0x92, 0x49, 0x24, 0x92, 0x49, 0x24, 0x92, 0x49 
};

unsigned char black_ball_sprite_mask[] = {
	0xe7, 0x81, 0x81, 0x00, 0x00, 0x81, 0x81, 0xe7
};
unsigned char black_ball_sprite_bmap[] = {
	0x00, 0x18, 0x3c, 0x76, 0x7e, 0x3c, 0x18, 0x00
};

static unsigned char white_ball_sprite_mask[] = {
	0xe7, 0x81, 0x81, 0x00, 0x00, 0x81, 0x81, 0xe7
};
static unsigned char white_ball_sprite_bmap[] = {
	0x00, 0x18, 0x3c, 0x76, 0x7e, 0x3c, 0x18, 0x00
};

SpriteFactory::SpriteFactory()
{
	;
}

void SpriteFactory::load_all()
{
	sprites[NULL_SPRITE] = new Sprite(0, 0, 0);
	sprites[WHITE_BALL] = new Sprite(8, 8, white_ball_sprite_bmap, white_ball_sprite_mask);
	sprites[BLACK_BALL] = new Sprite(8, 8, black_ball_sprite_bmap, black_ball_sprite_mask);
	sprites[GREY_PATTERN] = new Sprite(8, 8, grey_pattern_bmap);
	sprites[ZEBRA_PATTERN] = new Sprite(8, 8, zebra_pattern_bmap);
} 

const Sprite* SpriteFactory::get(SpriteKind sk)
{
	return sprites[sk];
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






