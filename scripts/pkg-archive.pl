#!/usr/bin/env perl
use strict;
use warnings;
use File::Basename qw(dirname);
use File::Path qw(make_path);

my ($source_dir, $out_file) = @ARGV;
if (!defined $source_dir || !defined $out_file) {
  die "usage: pkg-archive.pl <source-dir> <out-file>\n";
}
if (!-d $source_dir) {
  die "source directory not found: $source_dir\n";
}

my $out_dir = dirname($out_file);
if (defined $out_dir && length $out_dir && $out_dir ne ".") {
  make_path($out_dir) if !-d $out_dir;
}

my $cmd = "sattyre-packman pack \"$source_dir\" \"$out_file\"";
my $rc = system($cmd);
if ($rc != 0) {
  die "pack failed via sattyre-packman\n";
}

print "bundle created: $out_file\n";
