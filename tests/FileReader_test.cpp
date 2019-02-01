#include <iostream>
#include <string.h>
#include "FileReader.h"

using namespace std;

void assertTrue(bool c)
{
	if (!c) 
	{
		cerr << "condition failed" << endl;
		exit(1);
	}
}

void assertEqual(const char* s1, const char* s2)
{
	if (strcmp(s1, s2) != 0) 
	{
		cerr << "strings not equal! "
			<< "expected: \"" << s1
			<< "\", actual: \"" << s2
			<< "\"" << endl;
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	FileReader* r = new FileReader();
	r->open("FileReader_test.txt");

	assertEqual("abc", r->read_line());
	assertEqual("", r->read_line());
	assertEqual("def", r->read_line());
	assertEqual("", r->read_line());
	assertTrue(0 == r->read_line());
	assertTrue(r->at_end());
	
	delete r;
	return 0;
}
