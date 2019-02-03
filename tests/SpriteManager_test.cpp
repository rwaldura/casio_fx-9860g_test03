#include "Sprite.h"
#include <iostream>

extern "C" 
{
	#include "fxlib.h"
	void Bdisp_WriteGraph_VRAM(const DISPGRAPH *WriteGraph) {}
}

using namespace std;

void assertTrue(bool c, const char* mesg = "")
{
	if (!c) 
	{
		cerr << "condition failed: " << mesg << endl;
		exit(1);
	}
}

const char* TEST_FILE = "SpriteManager_test.txt";

int main(int argc, char* argv[])
{
	SpriteManager* m = new SpriteManager();
	
	int err = m->load_file(TEST_FILE);
	if (err) 
	{
		cerr << "Error " << err 
			<< " in loading file: " << TEST_FILE
			<< endl;
		return 1;
	}

	const Sprite* s = m->get(BLACK_PATTERN);
	assertTrue(s != 0, "null");
	assertTrue(s->kind == BLACK_PATTERN, "id");
	assertTrue(s->width == 8, "width");
	assertTrue(s->height == 8, "height");
	assertTrue(s->bitmap != 0, "bitmap");
	assertTrue(s->bitmap[0] == 0x0f, "bitmap0");
	assertTrue(s->bitmap[1] == 0xf0, "bitmap1");
	assertTrue(s->bitmap[2] == 0x01, "bitmap2");
	assertTrue(s->bitmap[3] == 0x80, "bitmap3");
	assertTrue(s->bitmap[4] == 0x11, "bitmap4");
	assertTrue(s->bitmap[5] == 0x88, "bitmap5");
	assertTrue(s->bitmap[6] == 0x55, "bitmap6");
	assertTrue(s->bitmap[7] == 0x00, "bitmap7");
	assertTrue(s->mask == 0, "mask");

	s = m->get(LIGHT_PATTERN);
	assertTrue(s != 0, "null");
	assertTrue(s->kind == LIGHT_PATTERN, "id");
	assertTrue(s->width == 8, "width");
	assertTrue(s->height == 8, "height");
	assertTrue(s->bitmap != 0, "bitmap");
	assertTrue(s->bitmap[0] == 0x01, "bitmap0");
	assertTrue(s->bitmap[1] == 0x02, "bitmap1");
	assertTrue(s->bitmap[2] == 0x04, "bitmap2");
	assertTrue(s->bitmap[3] == 0x08, "bitmap3");
	assertTrue(s->bitmap[4] == 0x10, "bitmap4");
	assertTrue(s->bitmap[5] == 0x20, "bitmap5");
	assertTrue(s->bitmap[6] == 0x40, "bitmap6");
	assertTrue(s->bitmap[7] == 0x80, "bitmap7");
	assertTrue(s->mask != 0, "mask");
	assertTrue(s->mask[0] == 0xff, "mask0");
	assertTrue(s->mask[1] == 0xff, "mask0");
	assertTrue(s->mask[2] == 0xff, "mask0");
	assertTrue(s->mask[3] == 0xff, "mask0");
	assertTrue(s->mask[4] == 0xff, "mask0");
	assertTrue(s->mask[5] == 0xff, "mask0");
	assertTrue(s->mask[6] == 0xff, "mask0");
	assertTrue(s->mask[7] == 0xff, "mask0");

	delete m;
	return 0;
}
