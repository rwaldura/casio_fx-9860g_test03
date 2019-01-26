#include "Sprite.h"

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