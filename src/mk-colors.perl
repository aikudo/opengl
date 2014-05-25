#!/usr/bin/perl
# $Id: mk-colors.perl,v 1.1 2014-05-25 13:00:52-07 - - $
use strict;
use warnings;

my %colors;
my $file = "rgb.txt";
open RGB_TXT, "<$file" or die "$0: $file: $!";
while (my $line = <RGB_TXT>) {
   $line =~ m/^\s*(\d+)\s+(\d+)\s+(\d+)\s+(.*)/
         or die "$0: invalid line: $line";
   my ($red, $green, $blue, $name) = ($1, $2, $3, $4);
   $name =~ s/\s+/-/g;
   $colors{$name} = [$red, $green, $blue];
}
close RGB_TXT;

print "const unordered_map<string,rgbcolor> colors = {\n";
printf "   {%-24s, rgbcolor (%3d, %3d, %3d)},\n",
                  "\"$_\"", @{$colors{$_}}
       for sort {lc $a cmp lc $b} keys %colors;
print "};\n";

