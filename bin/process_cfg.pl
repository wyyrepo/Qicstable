#! /usr/bin/perl

#This script processing qicstable.cfg file and sets all DOT parameters to NO

use Shell qw/rm cp mv/;

$pattern = '(CLASS_DIAGRAMS|HAVE_DOT|CLASS_GRAPH|COLLABORATION_GRAPH)';
$file = "../config/qicstable.cfg";

cp $file, "$file.bak";
# opening qicstable.cfg file
open (CONFIG,"$file.bak") or die "can't open qicstable.cfg file for reading";
open (CONFIG2,"> $file") or die "can't open qicstable.cfg file for writing";

while (<CONFIG>){
    s/($pattern\s+=)\s*YES/$1 NO/;
    print CONFIG2 $_;
}

