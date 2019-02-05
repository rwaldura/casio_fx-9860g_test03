/*
 * Read a file line by line: '\n' characters are end-of-line terminators.
 */

#ifndef FILE_READER_H
#define FILE_READER_H

#ifdef UNIT_TESTING
	#include <stdio.h>
#endif

class FileReader
{
public:
		static const int MAX_LINE = 64;

		FileReader(int max = MAX_LINE);
		~FileReader();
		int open(const char* file_name); // returns 0 on success
		void close();
		char* read_line();
		bool at_end();

	private:
		void fill_buffer();

		char* buffer;
		int i; // index into buffer
		int n; // current buffer size

#ifdef UNIT_TESTING
		FILE* file_handle;
#else
		int	file_handle;
#endif
};

#endif