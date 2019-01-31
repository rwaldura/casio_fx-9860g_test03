#include "Sprite.h"

extern "C" 
{
	#include "fxlib.h"
	void Bdisp_WriteGraph_VRAM(const DISPGRAPH *WriteGraph) {}
}

int main(int argc, char* argv[])
{
	SpriteManager* m = new SpriteManager();
	m->load_file("sprites.txt");
	delete m;
	return 0;
}
