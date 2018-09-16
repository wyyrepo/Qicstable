#! /bin/bash

# The Windows/Linux eval packaging is is binary-only.
# The tree must first be built on a Windows box (with
# CONFIG+=win_eval) and then copied to the current directory (the
# build tree must be named 'qicstable').
#
# lineval  <compiler + ver> <qt_ver> Linux eval (for Qt version <qt_ver>)
# wineval  <compiler + ver> <qt_ver> Windows eval (for Qt version <qt_ver>)


proj=qicstable
tdir=`pwd`"/../.."


#get system info
ker_name=`uname -s`
platform=`uname -i`
machine=`uname -m | tr [:upper:] [:lower:]`
gcc_v=`gcc -v 2>&1 | grep version | cut -d " " -f 3`
qit_v=`cat ../VERSION`
qt_v=`$QTDIR/bin/qmake -v 2>&1 | grep -m1 -o -P "\d\.\d\.\d" | head -1`

if [ $QICSTABLE_SUFFIX ]
then
    version=$QICSTABLE_SUFFIX
else
    version=$qit_v
fi


PACKAGE_NAME=""
PACKAGE_TYPE=""


function printUsage(){
    echo "./makeeval.sh <lineval/wineval> <compiler + ver> <qt_ver>"
    echo "lineval - make eval for Linux"
    echo "wineval - make eval for Windows"
}



function getEvalPackageName() {


    case $1 in
    wineval)
        type="ia32-win"
        ;;
    lineval)
        type="x86-linux"
        ;;
    *)
        echo "Invalid parameter"
        exit 1
    esac


    PACKAGE_TYPE=$1

    PACKAGE_NAME=${proj}"-"${version}"-"${type}"-"$2"-qt_"$3"-eval"

    return 0
}



########################################################
##############           MAIN            ###############
########################################################


if [ -z $1 ]
then
    echo "No input parameters"
    printUsage
    exit 1
fi


#######################################################
echo "Defining package name...."

if [ $USER != "tinderbox" ]
then
    if [ $# -ne 3 ]
    then
        echo "Illegal count of args..."
        printUsage
        exit 1
    fi

    compiler=$2
    qt_ver=$3
    qt_v=$3

else
    if [[  $1 != "lineval" && $1 != "wineval"  ]]
    then
        printUsage
        exit 1
    fi

    compiler=$gcc_v
    qt_ver=$qt_v
fi

getEvalPackageName "$1" $compiler $qt_v

if [ $? -eq 1 ]
then
    echo "getPackageName returned code 1"
    exit 1
fi

echo "Package name: "$PACKAGE_NAME

echo "Updating version variables"
./update_version

echo "Processing documentation config"
./process_cfg.pl


cd $tdir
if [ ! -d package ]
then
    mkdir package
fi


cp -r $tdir/$proj $tdir/package

tdir=$tdir/package
top=$tdir/$proj

#
# Make tar dir
#
mkdir -p $tdir

#cd $tdir


cd $top
cd ..
echo `pwd`
find . -name .svn -print | xargs rm -rf
#find . -name .cvsignore -print | xargs rm -f

#
# Get rid of tests
#
cd $top
rm -rf tests

#
# Get rid of internal examples
#
cd $top
rm -rf examples/tabledemo


#
# When doing the Windows eval, copying the tree from Windows to
# NFS loses the execute bits on the scripts
#
if [ $PACKAGE_TYPE = "wineval" ]
then
   cd $top/bin
   for f in *
   do
      dos2unix $f
   done

   chmod 755 $top/bin/*
fi


#
# Windows eval may have examples built.  We need to remove them
#
echo "Eval cleaning ....."
cd $top/examples
if [ $PACKAGE_TYPE = "lineval" ]
then
   find . -type f -perm /+g+x,o+x -print | xargs rm -f
   cd $top
   find . -name \*.vcproj -print | xargs rm -rf
   find . -name \*.sln -print | xargs rm -rf
fi

cd $top
find . -type d -name moc -print | xargs rm -rf
find . -type d -name obj -print | xargs rm -rf
find . -type d -name debug -print | xargs rm -rf
find . -type d -name .build -print | xargs rm -rf
find . -name \*debug.\* -print | xargs rm -rf
find . -type d -name release -print | xargs rm -rf
find . -name moc_\*.cpp -print | xargs rm -f
find . -name \*.moc -print | xargs rm -rf
find . -name \*.exe -print | xargs rm -f
find . -name \*.obj -print | xargs rm -rf
find . -name \*.ilk -print | xargs rm -f
find . -name \*.pdb -print | xargs rm -f
find . -name \*.rc -print | xargs rm -f
find . -name \*.res -print | xargs rm -f
find . -name \*.o -print | xargs rm -f

cd $top
find . -name Makefile -print | xargs rm -f
find . -name Makefile\* -print | xargs rm -f
#find . -type d -name designer -print | xargs rm -rf


#
# Remove plugins
#
cd $top
mkdir -p plugins
cd $top/plugins
rm -rf *
cd $top/designer
rm -f *.obj
rm -f *.o
cd $top/
rm -f *.o

cd $top
rm -rf src

#
# Nuke TODO file for real release
#
cd $top
rm -f TODO

#
# Remove unneeded license/install files
#
cd $top
mkdir tmpinst

mv INSTALL* tmpinst
if [ $PACKAGE_TYPE = "wineval" ]
then
    cp tmpinst/INSTALL.WINEVAL INSTALL
else
    cp tmpinst/INSTALL.UNIXEVAL INSTALL
fi

rm -rf tmpinst

cd $top
mkdir tmpinst
mv LICENSE* tmpinst
cp tmpinst/LICENSE.EVAL LICENSE

rm -rf tmpinst

rm -f qicstable.pro
mv qicstable_wineval.pro qicstable.pro

#
# Convert pyqicstable commercial to evaluation
#
cd $top
# Also replace pyqicstable license file
pydir=$top/addons/pyqicstable
rm -f $pydir/LICENSE
cp -f LICENSE $pydir/LICENSE

cd $pydir
patch -p0 < $top/bin/pyqicstable_eval.patch
mv $pydir/sip/pyqicstable-commercial.sip $pydir/sip/pyqicstable-eval.sip


#
# Preprocess files (remove commercial features, or maybe just the tags)
# Get rid of unneeded files
#

cd $top/include
for f in *.h
do
    $top/bin/./remove_comm_tags < $f > $f.new
    rm -f $f
    mv $f.new $f
done


$top/bin/generate_docs

#
# Preprocess files to remove doxygen comments
#
cd $top/include
for f in *.h
do
    $top/bin/remove_dox_comments < $f > $f.new
    rm -f $f
    mv $f.new $f
done

#
# When doing a windows eval, we want to re-copy the header files
# to $top/include, now that they have been processed
#
cd $top
rm -f include/moc_*.*


#
# Get rid of unneeded directories
#
cd $top
rm -rf bin config Include designer adapter pack  plugins VERSION
rm -f doxyLog

#
# Fixup doc directory
#
cd $top
mv doc/html html
mv doc/QicsTable.dcf QicsTable.dcf
mv doc/README README_DOC
rm -rf doc
mkdir doc
mv html doc
mv QicsTable.dcf doc/QicsTable.dcf
mv README_DOC doc/README


#
# Make Windows zip file
#
cd $tdir

if [ $PACKAGE_TYPE = "wineval" ]
then
  mv $proj $PACKAGE_NAME
  zip -r ${PACKAGE_NAME}.zip $PACKAGE_NAME
  mv $PACKAGE_NAME $proj
else
  rm -rf ${PACKAGE_NAME}.tar.gz
  mv $proj $PACKAGE_NAME
  tar cvf ${PACKAGE_NAME}.tar $PACKAGE_NAME
  gzip ${PACKAGE_NAME}.tar
  mv $PACKAGE_NAME $proj
fi


# Finally check for commercial stuff
# bindir ==  $tdir/../qicstable/bin/
$tdir/../qicstable/bin/check_nonfree_stuff.sh

echo remove working dir .........
cd $top
cd ..
rm -rf $proj
