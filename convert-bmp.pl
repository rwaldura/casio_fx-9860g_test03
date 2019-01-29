#!/usr/bin/perl
#
# Read a monochrome BMP file and output its content as an array of bytes,
# suitable for inclusion into a C program. 
#
# This is really to create sprites for the Casion fx-9860G calculator;
# until I find a way to bundle and read BMPs by program. :(
#
# Note 
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
	my($img) = @_;
	my $pixel_bits = '';
	
	#$img->view_ascii();

	# read row by row
	for (my $y = 0; $y < $img->{Height}; $y++)
	{
		for (my $x = 0; $x < $img->{Width}; $x++)
		{
			my $color = $img->xy($x, $y);
			#my($r, $g, $b) = $img->xy_rgb($x, $y);
			#my $index = $img->xy_index($x, $y);
			#warn "x=$x, y=$y : i=$index, c=$color, (r,g,b)=($r,$g,$b)";
		
			# reverse BMP color logic for monochrome LCD screen: 
			# black pixels (zeroes) become ones (lit)
			$pixel_bits .= ($color == 0) ? '1' : '0';
		}	
	}
	
	return $pixel_bits;
}

sub print_bitmap_array
{
	my($hex_bytes, $img_width, $comment) = @_;
	
	my $n = 0;
	my $c_array = '';
	foreach (@$hex_bytes)
	{
		$c_array .= "\n\t" if ($n++ % ($img_width / 8) == 0);
		$c_array .= "0x$_, ";
	}
	$c_array =~ s/, $//; # zap final comma
	#warn $n;

	print "/* $comment */
static const unsigned char bitmap_data[] = {$c_array
};\n";
	
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
	
	# read the BMP into memory
	my $img = new Image::BMP('file' => $bmp_file);
	$img->load();
	$img->close();
	
	my $pixel_bits = read_bmp_bits($img);
	#warn $pixel_bits;
	
	# target system is big-endian
	my @hex_bytes = unpack("(A2)*", unpack('H*', pack('B*', $pixel_bits)));
	#warn @hex_bytes;

	# if the image is exactly twice as tall as it is wide,
	# split bytes in half: first half is actual bitmap, second is the mask
	my $h2 = $img->{Height} / 2;
	if ($img->{Width} == $h2)
	{
		my @bitmap_bytes = @hex_bytes[0 .. $img->{Height} - 1];
		my @mask_bytes = @hex_bytes[$img->{Height} .. 2 * $img->{Height} - 1];
		print_bitmap_array(\@bitmap_bytes, $img->{Width}, "$bmp_file bitmap $img->{Width}x$h2");
		print_bitmap_array(  \@mask_bytes, $img->{Width}, "$bmp_file mask $img->{Width}x$h2");		
	}
	else
	{
		print_bitmap_array(\@hex_bytes, $img->{Width}, "$bmp_file $img->{Width}x$img->{Height}");		
	}
}


