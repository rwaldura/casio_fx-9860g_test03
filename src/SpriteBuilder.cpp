/*
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "FileReader.h"
#include "Sprite.h"

static const int MAX_BITMAP_SIZE = 2 * 16 * 16; // in pixels
static const int BPC = 8; // bits per char
// 1 pixel == 1 bit

/*
 * Turn the string "#..##" into the bits 10011.
 */
unsigned char* SpriteBuilder::parse_bitmap_string(int width, int height, const char* s)
{
	unsigned char* bitmap = new unsigned char[width * height / BPC](); // zero-initialized array

	for (int i = 0; i < width * height; i++)
	{
		// lit black pixel is marked by '#' character
		if (s[i] == '#')
		{
			bitmap[i / BPC] |= 1 << (7 - i % BPC);			
		}
	}

	return bitmap;
}

/*
 * Instantiate a new sprite. 
 */
const Sprite* SpriteBuilder::_build_sprite(int id, int width, int height, const char* bitmap_str)
{
	const Sprite* s;

	if (strlen(bitmap_str) == width * height)
	{
		// parse the bitmap definition into actual bits
	  	unsigned char* bitmap = parse_bitmap_string(width, height, bitmap_str);
		s = new Sprite((SpriteKind) id, width, height, bitmap);	
	}
	else if (strlen(bitmap_str) == 2 * width * height)
	{
		height /= 2;

		// parse the bitmap definition into actual bits
	  	unsigned char* bmap = parse_bitmap_string(width, height, bitmap_str);	
		unsigned char* mask = parse_bitmap_string(width, height, bitmap_str + width * height);

		s = new Sprite((SpriteKind) id, width, height, bmap, mask);		
	}
	else // invalid bitmap definition
	{
		s = 0;
	}
		
	return s;
}

/*
 * Read one sprite definition from the reader, and return the sprite instance. 
 */
const Sprite* SpriteBuilder::build_sprite()
{
	int id = 0;
	int width = 0, height = 0;
	char* bitmap_str = new char[MAX_BITMAP_SIZE]();
	bool end = false;

	// read one sprite definition from the file
	char* line;
	while (!end && (line = reader.read_line()))
	{
		// skip comments: lines that start with "//"
		if (strncmp(line, "//", 2) == 0) 
		{
			// skip
		}
		// read the sprite ID
		else if (strncmp(line, "ID=", 3) == 0)
		{
			id = atoi(line + 3);
		}
		// blank line signals end of sprite
		else if (strlen(line) == 0)
		{
			end = true;
		}
		// read bitmap
		else
		{
			if (width == 0) width = strlen(line);
			if (strlen(line) + strlen(bitmap_str) > MAX_BITMAP_SIZE) return 0; // bitmap is too large
			strcat(bitmap_str, line);
			height += 1;
		}

		delete line;
	}

	const Sprite* s = _build_sprite(id, width, height, bitmap_str);
	delete [] bitmap_str;
	return s;
}
