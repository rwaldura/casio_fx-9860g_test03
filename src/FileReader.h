/*
 * Read a file line by line: '\n' characters are end-of-line terminators.
 */

#ifndef FILE_READER_H
#define FILE_READER_H

#include <stddef.h>

#ifdef UNIT_TESTING
	#include <cstdio>
#endif

class FileReader
{
public:
		static const size_t MAX_LINE = 64;

		FileReader(size_t max = MAX_LINE);
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