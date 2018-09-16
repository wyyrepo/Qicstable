#!/bin/sh

#
# Used by makeeval.sh
# Check files before packaging for Commercial stuff which shouldn't be published.
#

pwd
ls -l

echo ""
echo "***************************************************************"
echo ""
echo "		Pay attention to the following files"
echo "		They shouldn't be there"
echo ""
echo "***************************************************************"
echo ""

find . -name \*evaldialog\* -print
find . -name \*src\* -print
find . -name \*commercial\* -print
find . -name \*COMMERCIAL\* -print
find . -name \*.o -print
find . -name \*.pyc -print
find . -name \*.obj -print
find . -name \*.sln -print
find . -name \*.vcproj -print
find . -name \*.sh -print
find . -name \*.pl -print
find . -name \*.patch -print
find . -name \*.diff -print
find . -name \*.doc -print
find . -name \*test\* -print
find . -name \*tmp\* -print
find . -name \*temp\* -print
find . -name \*.log\* -print
find . -name \*~\* -print
find . -name \*\#\* -print
find . -name \*SVN\* -print
find . -name \*svn\* -print
find . -name \*build\* -print
#find . -name ?\*.\*.\* -print
find . -name \*.exe -print
find . -name \*debug\* -print
find . -name \*release\* -print
find . -name \*moc\* -print
find . -name obj -print


echo "***************************************************************"
echo ""
