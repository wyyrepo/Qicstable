#! /bin/sh

gcc -o stype stype.c

cp stype ../../package
cd ../../package

str=`echo *.tar.gz | head -1`

echo Preparing for uploading $str... ok!

( sleep 60 ; ./stype /dev/tty `echo -e dxle5zUE\\\\r` )&
sftp -v files@bedford4.ics.com   <<END_SCRIPT
cd QicsTable/tinderbox_evals
#binary
rm $str
put `pwd`/$str ./$str
chmod 666 $str
END_SCRIPT

cd ../qicstable/bin

exit 0

