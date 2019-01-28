#!/usr/bin/perl
#
# Read a monochrome BMP file and output its content as an array of bytes,
# suitable for inclusion into a C program. 
#
# This is really to create sprites for the Casion fx-9860G calculator;
# until I find a way to bundle and read BMPs by program. :(
#
# Note 1
# Bitmaps are usually read from left to right, and bottom to top
# (x from 0 to width, y from height to 0).
# Here we flip the image, such that pixels are listed from top to bottom.
#
# Note 2
# We reverse the colors: a black pixel in the BMP, becomes white in the 
# output. That's because the fx-9860G has a monochrome LCD screen, where 
# bit 1 = pixel lit = black color. Unlike a regular computer display, where 
# bit 0 = pixel off = black.
#

use strict;
use Image::BMP;

#
# Read the BMP data, and return it as a string of bits: 0 or 1.
#
sub read_bmp_bits
{
	my($bmp_file) = @_;
	
	my $pixel_bits = '';
	
	my $img = new Image::BMP();
	$img->open_file($bmp_file);

	#$img->view_ascii();
	
	# reverse order: read from top to bottom
	for (my $y = 0; $y < $img->{Height}; $y++)
	# for (my $y = $img->{Height} - 1; $y >= 0; $y--)
	{
		for (my $x = 0; $x < $img->{Width}; $x++)
		{
			my $color = $img->xy($x, $y);
			my($r, $g, $b) = $img->xy_rgb($x, $y);
			my $index = $img->xy_index($x, $y);
			warn "x=$x, y=$y : i=$index, c=$color, (r,g,b)=($r,$g,$b)";
		
			# reverse BMP color logic: black pixels become ones (lit).
			$pixel_bits .= ($r == 0 && $g == 0 && $b == 0) ? '1' : '0';
		}	
	}

	$img->close();
	
	return $pixel_bits;
}

#
# main function: process files found on the command line.
# 
foreach (@ARGV)
{
	unless (/\.bmp$/i) 
	{
		warn "Skipping $_: file does not end in \".bmp\"";
		next;
	}
	
	my $bmp_file = $_;
	my $pixel_bits = read_bmp_bits($bmp_file);
	warn $pixel_bits;
	
	# target system is big-endian
	my $hex = unpack('H*', pack('B*', $pixel_bits));
	warn $hex;

	my $hex_bytes = '';
	my $n;
	while ($hex =~ /(..)/g)
	{
		$hex_bytes .= "0x$1, ";
		$hex_bytes .= "\n\t" if (++$n % 16 == 0);
	}
	$hex_bytes =~ s/, $//; # zap last comma

	print "
/* $bmp_file */
static const unsigned char bitmap_data[] = {
	$hex_bytes
};
";
}


