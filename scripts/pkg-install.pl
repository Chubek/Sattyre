#!/usr/bin/env perl
use strict;
use warnings;
use File::Basename qw(basename);
use File::Path qw(make_path);
use File::Spec;

my ($url, $bundle_name, $root_arg) = @ARGV;
if (!defined $url || !defined $bundle_name) {
  die "usage: pkg-install.pl <bundle-url> <bundle-name> [install-root]\n";
}

my $home = $ENV{SATTYRE_HOME} || File::Spec->catdir($ENV{HOME}, ".sattyre");
my $root = defined $root_arg ? $root_arg : $home;
my $cache_dir = File::Spec->catdir($root, "cache", "downloads");
make_path($cache_dir) if !-d $cache_dir;

my $safe_name = basename($bundle_name);
my $bundle_path = File::Spec->catfile($cache_dir, $safe_name);

my $wget_cmd = "wget -O \"$bundle_path\" \"$url\"";
my $wget_rc = system($wget_cmd);
if ($wget_rc != 0) {
  die "download failed with wget: $url\n";
}

my $install_cmd = "sattyre-packman install \"$bundle_path\" --root \"$root\" --overwrite";
my $install_rc = system($install_cmd);
if ($install_rc != 0) {
  die "install failed: $bundle_path\n";
}

print "installed bundle: $bundle_path\n";
print "install root: $root\n";
