#include "Sprite.h"

extern "C" 
{
	#include "fxlib.h"
	void Bdisp_WriteGraph_VRAM(const DISPGRAPH *WriteGraph) {}
}

using namespace std;

int main(int argc, char* argv[])
{
	SpriteManager* m = new SpriteManager();
	
	int err = m->load_file("SpriteManager_test.txt");
	if (err) 
	{
		cerr << "Error " << err 
			<< " in loading file: SpriteManager_test.txt"
	}

	delete m;
	return 0;
}
