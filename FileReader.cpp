/*
 *
 */
 
#include "FileReader.h"
#include <stdlib.h>
#include <string.h>

#ifdef UNIT_TESTING
#include <stdio.h>
#else
#include "fxlib.h"
#endif 

FileReader::FileReader()
{
	file_handle = 0;
	i = -1; // index into buffer
	n = -1; // current buffer size
}

FileReader::~FileReader()
{
	close();
}

void FileReader::close()
{
	if (file_handle)
	{
#ifdef UNIT_TESTING
		fclose(file_handle);
#else
		Bfile_CloseFile(file_handle);
#endif
	}

	file_handle = 0;
}

/*
 * Returns zero on success, error code otherwise
 */
int FileReader::open(const char* file_name)
{
#ifdef UNIT_TESTING
	file_handle = fopen(file_name, "r");
	return (file_handle) ? 0 /* no error */ : 1; // errno; 
#else
	file_handle = Bfile_OpenFile((const FONTCHARACTER*) file_name, _OPENMODE_READ);
	return (file_handle > 0) 0 /* no error */ : file_handle; 
#endif 
}

void FileReader::fill_buffer()
{
	i = 0;
	n =  
#ifdef UNIT_TESTING
		fread(buffer, sizeof(char), BUFSIZ, file_handle);
#else
		Bfile_ReadFile(file_handle, buffer, BUFSIZ, -1);
#endif
}

/*
 * Returns 0 on empty line, or EOF.
 */
char* FileReader::read_line()
{
	if (at_end()) return 0;

	if (i == n)
	{
		fill_buffer();
	}

	bool eol = false;
	int j;
	for (j = i; j < n; j++)
	{
		if (buffer[j] == '\n')
		{
			eol = true;
			break;
		}
	}

	char* line = 0;
	if (eol && j > i)
	{
		line = new char[1 + (j - i)](); // + 1 for final '\0'
		strncpy(line, buffer + i, j - i);
	}

	return line;
}

bool FileReader::at_end()
{
	return n == 0;
}

#ifdef UNIT_TESTING
#include <iostream>

int main(int argc, char* argv[])
{
	FileReader* r = new FileReader();
	r->open("sprites/sprites.txt");

	char* l; int n = 0;
	while (l = r->read_line())
	{
		std::cout 
			<< (n++) 
			<< " - " 
			<< l 
			<< std::endl;
	}

	delete r;
	return 0;
}
#endif
