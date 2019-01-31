#include <iostream>
#include "FileReader.h"

int main(int argc, char* argv[])
{
	FileReader* r = new FileReader();
	r->open("sprites.txt");

	char* l; int n = 0;
	while (l = r->read_line())
	{
		std::cout 
			<< (n++) 
			<< " - " 
			<< l 
			<< std::endl;
		delete l;
	}

	delete r;
	return 0;
}
