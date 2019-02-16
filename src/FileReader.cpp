/*
 * Implementation of class FileReader.
 */
 
#include <stdlib.h>
#include <string.h>
#include "FileReader.h"

extern "C"
{
	#include "fxlib.h"
}

#ifdef UNIT_TESTING
	#include <cerrno>
	#include <cstdio>
#endif 

FileReader::FileReader(size_t max)
{
	buffer = new char[max];
	file_handle = 0;
	i = -1; // index into buffer
	n = -1; // current buffer size
}

FileReader::~FileReader()
{
	close();
	delete [] buffer;
	buffer = 0;
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
 * Convert a C string to a Casio FONTCHARACTER string.
 * The returned value is freshly allocated, and should therefore 
 * be disposed of properly, i.e. deleted.
 */
static FONTCHARACTER* strtoFONTCHARACTER(const char* s)
{
	FONTCHARACTER* f = new FONTCHARACTER[1 + strlen(s)];

	// also copy the terminating '\0'
	for (size_t j = 0; j < 1 + strlen(s); j++)
	{
		f[j] = s[j];
	}

	return f;
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
	FONTCHARACTER* fc = strtoFONTCHARACTER(file_name);
	file_handle = Bfile_OpenFile(fc, _OPENMODE_READ);
	delete [] fc;
	return (file_handle > 0) ? 0 /* no error */ : file_handle; 
#endif 
}

inline void FileReader::fill_buffer()
{
	i = 0;
	n =  
#ifdef UNIT_TESTING
		fread(buffer, sizeof(char), MAX_LINE, file_handle);
#else
		Bfile_ReadFile(file_handle, buffer, MAX_LINE, -1);
#endif
}

/*
 * Returns the next line, without the terminal '\n'.
 * Zero-length strings are returned for blank lines.
 * Returns 0 on EOF.
 * The string returned is freshly allocated, and will need to be
 * disposed of, with "delete".
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
		line = new char[j - i](); // zero-initialized
		strncpy(line, buffer + i, j - i - 1); // -1 to skip the final '\n'
	}

	// reset our index into the buffer, right past the newline
	i = j;
	
	return line;
}

bool FileReader::at_end()
{
	return n == 0;
}
