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
		FileReader();
		~FileReader();
		int open(const char* file_name); // returns 0 on success
		void close();
		char* read_line();
		bool at_end();

	private:
		void fill_buffer();

		static const int MAX_LINE = 64;
		char buffer[MAX_LINE];
		int i; // index into buffer
		int n; // current buffer size

#ifdef UNIT_TESTING
		FILE* file_handle;
#else
		int	file_handle;
#endif
};

#endif