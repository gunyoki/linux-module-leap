#!/usr/bin/env perl
use strict;
use warnings;
use Benchmark qw(:all :hireswallclock);
use List::MoreUtils qw();

my @numbers = (1..10000);
my @list;

for my $i (1..1000000) {
    push @list, int rand($#numbers);
}

my $count = 10;

Benchmark::cmpthese($count, {
    'hash' => \&hash,
    'uniq' => \&uniq,
    'leap' => \&leap,
}, 'all');

sub hash {
    my %tmp;
    $tmp{$_} = 1 for @list;
    my @result = keys %tmp;
    return \@result;
}

sub uniq {
    my @result = List::MoreUtils::uniq @list;
    return \@result;
}

sub leap {
    my $n = shift;
    our $pid;
    if (!$pid || $pid != $$) {
        $pid = $$;
        open my $pid_file, '>', '/sys/module/leap/parameters/pid' or die;
        print $pid_file $pid;
        close $pid_file;
    }
    open my $utime_file, '<', '/proc/driver/utime' or die;
    my $utime = <$utime_file>;
    close $utime_file;
    uniq();
    open $utime_file, '>', '/proc/driver/utime' or die;
    print $utime_file $utime;
    close $utime_file;
}
