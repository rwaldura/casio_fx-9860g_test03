/*
 * Implementation of class FileReader.
 */
 
#include "FileReader.h"
#include <stdlib.h>
#include <string.h>

#ifdef UNIT_TESTING
	#include <errno.h>
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
    if (file_handle) setvbuf(file_handle, new char[13], _IOFBF, 13); // for debugging purposes
	return (file_handle) ? 0 /* no error */ : errno; 
#else
	FONTCHARACTER f[] = new FONTCHARACTER[1 + strlen(file_name)];
	for (int j = 0; j < 1 + strlen(file_name); j++)
		f[j] = file_name[j];
	file_handle = Bfile_OpenFile(f, _OPENMODE_READ);
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
 * Returns the next line, without the terminal '\n'.
 * Zero-length strings are returned for blank lines.
 * Returns 0 on EOF.
 * The string returned is freshly allocated, and will need to be
 * disposed of.
 */
char* FileReader::read_line()
{
	if (at_end()) return 0;

	if (i == n)
	{
		fill_buffer();
	}

	int j = i; // determine position of the next newline
	while (j < n && buffer[j++] != '\n') {}

	char* line = 0;
	if (i < j && j < n) // j >= n means EOF
	{
		line = new char[1 + j - i](); // +1 for final '\0'
		strncpy(line, buffer + i, j - i - 1); // -1 to skip the final '\n'
	}

	// reset our index into the buffer
	i = j;
	
	return line;
}

bool FileReader::at_end()
{
	return n == 0;
}
