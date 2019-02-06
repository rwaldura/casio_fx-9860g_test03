#include <cassert>
#include <iostream>
#include <cstring>
#include <string>
#include "FileReader.h"

using namespace std;

void assertEqual(const char* s1, const char* s2, const char* mesg = "")
{	
	string s = string("expected: \"") + s1
		+ "\", actual: \"" + s2
		+ "\": " + mesg;

	assert(s1 != 0 && s2 != 0 && strcmp(s1, s2) == 0);
}

void FileReader_test()
{
	FileReader r;
	int e = r.open("FileReader_test.txt");
	assert(e == 0);

	assertEqual("abc_def_ghi_jkl", r.read_line());
	assertEqual("", r.read_line());
	assertEqual("###_***_xyz", r.read_line());
	assert(0 == r.read_line());
	assert(0 == r.read_line());
	assert(r.at_end());
}

int main(int argc, char* argv[])
{
	FileReader_test();
	return 0;
}
