#include <string.h>
#include <iostream>
#include <string>
#include "FileReader.h"

using namespace std;

void assertTrue(bool c, const char* mesg = "")
{
	if (!c) 
	{
		cerr << "condition failed: " << mesg << endl;
		exit(1);
	}
}

void assertEqual(const char* s1, const char* s2, const char* mesg = "")
{	
	string s = string("expected: \"") + s1
		+ "\", actual: \"" + s2
		+ "\": " + mesg;

	assertTrue(s1 != 0 && s2 != 0 && strcmp(s1, s2) == 0);
}

int main(int argc, char* argv[])
{
	FileReader* r = new FileReader();
	r->open("FileReader_test.txt");

	assertEqual("abc_def_ghi_jkl", r->read_line());
	assertEqual("", r->read_line());
	assertEqual("###_***_xyz", r->read_line());
	assertTrue(0 == r->read_line(), "0 line");
	assertTrue(0 == r->read_line(), "0 line again");
	assertTrue(r->at_end(), "at end");
	
	delete r;
	return 0;
}
