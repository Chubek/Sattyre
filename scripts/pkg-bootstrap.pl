#!/usr/bin/env perl
use strict;
use warnings;
use File::Path qw(make_path);
use File::Spec;

my $home = $ENV{SATTYRE_HOME} || File::Spec->catdir($ENV{HOME}, ".sattyre");
my @dirs = (
  $home,
  File::Spec->catdir($home, "bin"),
  File::Spec->catdir($home, "lib"),
  File::Spec->catdir($home, "lib", "solvers"),
  File::Spec->catdir($home, "lib", "plugins"),
  File::Spec->catdir($home, "lib", "lua"),
  File::Spec->catdir($home, "share"),
  File::Spec->catdir($home, "share", "packages"),
  File::Spec->catdir($home, "examples"),
  File::Spec->catdir($home, "cache"),
  File::Spec->catdir($home, "cache", "downloads"),
  File::Spec->catdir($home, "tmp"),
  File::Spec->catdir($home, "logs"),
);

for my $dir (@dirs) {
  make_path($dir);
  print "created: $dir\n";
}

print "sattyre home initialized at: $home\n";
