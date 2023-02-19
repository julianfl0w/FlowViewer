#! /usr/bin/perl -w
# MD5: df9eb1c770e990e0f60bb14095792254
# TEST: ./rwtotal --sport --skip-zero ../../tests/data-v6.rwf ../../tests/empty.rwf ../../tests/data-v6.rwf

use strict;
use SiLKTests;

my $rwtotal = check_silk_app('rwtotal');
my %file;
$file{v6data} = get_data_or_exit77('v6data');
$file{empty} = get_data_or_exit77('empty');
check_features(qw(ipv6));
my $cmd = "$rwtotal --sport --skip-zero $file{v6data} $file{empty} $file{v6data}";
my $md5 = "df9eb1c770e990e0f60bb14095792254";

check_md5_output($md5, $cmd);
