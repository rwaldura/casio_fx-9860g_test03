#include "Sprite.h"
#include <iostream>
#include <list>
#include <stdlib.h>
#include <cassert>

extern "C"
{
	#include "fxlib.h"
	void Bdisp_WriteGraph_VRAM(const DISPGRAPH *WriteGraph)
	{
		assert(false);
	}
}

using namespace std;

void test_parse_bitmap(const SpriteBuilder& sb)
{
	unsigned char* bytes = sb.parse_bitmap_string(8, 2, "........########");
	assert(bytes[0] == 0x00);
	assert(bytes[1] == 0xFF);

	bytes = sb.parse_bitmap_string(8, 2, "....########....");
	assert(bytes[0] == 0x0F);
	assert(bytes[1] == 0xF0);

	bytes = sb.parse_bitmap_string(8, 2, "#.#.#.#..#.#.#.#");
	assert(bytes[0] == 0xAA);
	assert(bytes[1] == 0x55);
}

void test_build_sprites(const SpriteBuilder& sb)
{
	list<const Sprite*> sprites;

	while (!sb.is_done())
	{
		const Sprite* s = sb.build_sprite();
		sprites.push_back(s);	
	}
	
	const Sprite* s;
	s = sprites.front(); sprites.pop_front();
	assert(s);
	assert(s->kind == BLACK_PATTERN);
	assert(s->width == 8);
	assert(s->height == 8);
	assert(s->bitmap != 0);
	assert(s->bitmap[0] == 0x0f);
	assert(s->bitmap[1] == 0xf0);
	assert(s->bitmap[2] == 0x01);
	assert(s->bitmap[3] == 0x80);
	assert(s->bitmap[4] == 0x11);
	assert(s->bitmap[5] == 0x88);
	assert(s->bitmap[6] == 0x55);
	assert(s->bitmap[7] == 0x00);
	assert(s->mask == 0);

	s = sprites.front(); sprites.pop_front();
	assert(s);
	assert(s->kind == LIGHT_PATTERN);
	assert(s->width == 8);
	assert(s->height == 8);
	assert(s->bitmap != 0);
	assert(s->bitmap[0] == 0x01);
	assert(s->bitmap[1] == 0x02);
	assert(s->bitmap[2] == 0x04);
	assert(s->bitmap[3] == 0x08);
	assert(s->bitmap[4] == 0x10);
	assert(s->bitmap[5] == 0x20);
	assert(s->bitmap[6] == 0x40);
	assert(s->bitmap[7] == 0x80);
	assert(s->mask);
	assert(s->mask[0] == 0xff);
	assert(s->mask[1] == 0xff);
	assert(s->mask[2] == 0xff);
	assert(s->mask[3] == 0xff);
	assert(s->mask[4] == 0xff);
	assert(s->mask[5] == 0xff);
	assert(s->mask[6] == 0xff);
	assert(s->mask[7] == 0xff);	
}

void SpriteBuilder_test(const char* filename)
{
	FileReader r;
	SpriteBuilder sb = SpriteBuilder(r);

	test_parse_bitmap(sb);

	int err = r.open(filename);
	assert(err == 0);
	
	test_build_sprites(sb);
}

int main(int argc, char* argv[])
{
	SpriteBuilder_test("SpriteBuilder_test.txt");
	return 0;
}

