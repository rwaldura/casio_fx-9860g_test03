/*
 * Read a file line by line, ignoring all '\n' characters.
 */

#include <stdio.h>

#ifndef FILE_READER_H
#define FILE_READER_H

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