#!/bin/sh 

TOP=`pwd`
TOP_TABLE=$TOP/../
TMP=$TOP/temp
CREATED=$TOP/created

#get system info
ker_name=`uname -s`
platform=`uname -i`
machine=`uname -m | tr [:upper:] [:lower:]`
gcc_v=`gcc -v 2>&1 | grep version | cut -d " " -f 3`
qit_v=`cat $TOP_TABLE/VERSION`
qt_v=`$QTDIR/bin/qmake -v 2>&1 | grep -o -P "\d\.\d\.\d"`

name="qicstable-"$qit_v"-"$machine"-"$gcc_v"-qt_"$qt_v
eval_name=$name"-eval"
bin_name=$name"-bin"

#make directory for created packages
rm -fr $CREATED
mkdir -p $CREATED

mkdir -p $TMP
echo "Cleaning $TMP dir..."
rm -fr $TMP/*

#clean first
echo "Running make clean..."
cd $TOP_TABLE && $QTDIR/bin/qmake && make distclean &> /dev/null

#
#creating with sources
#

#copy src and includes
echo "Creating directories..."
cd $TMP && mkdir -p include && 
  mkdir -p src && 
  mkdir -p examples && 
  mkdir -p doc
cp -fr $TOP_TABLE/include $TMP
cp -fr $TOP_TABLE/src $TMP
cp -fr $TOP_TABLE/examples $TMP

#generate documentation
echo "Generating documentation..."
cp -fr $TOP/qicstable.cfg $TMP/doc/
cd $TMP/doc && doxygen qicstable.cfg &> /dev/null

echo "Deleting unnecessary files"
#delete moc and obj directories
cd $TMP && rm -fr `find . -type d | grep moc`
cd $TMP && rm -fr `find . -type d | grep obj`
cd $TMP && rm -fr `find . -type d | grep CVS`
cd $TMP && rm -fr `find . -type f | grep .cvsignore`

echo "Compressing files to $eval_name..."
cd $TMP && tar -cf $eval_name".tar" * && bzip2 -9 $eval_name".tar"
cd $TMP && mv $eval_name* $CREATED

echo "Done!"
sleep 5

#
#creating binary
#

#clean temp
rm -fr $TMP/*

#build first
echo "Building qicstable..."
cd $TOP_TABLE && $QTDIR/bin/qmake && make &> /dev/null

cd $TMP && mkdir -p include &&
    mkdir -p examples

echo "Copying include directory..."
cp -fr $TOP_TABLE/include/* $TMP/include

echo "Copying lib directory..."
mv -f $TOP_TABLE/lib $TMP/

#call clean 
echo "Cleaning project..."
cd $TOP_TABLE && make distclean &> /dev/null

echo "Copying examples directory..."
cp -fr $TOP_TABLE/examples/* $TMP/examples/


echo "Deleting unnecessary files..."
#delete moc and obj directories (damn that qmake)
cd $TMP && rm -fr `find . -type d | grep moc`
cd $TMP && rm -fr `find . -type d | grep obj`
cd $TMP && rm -fr `find . -type d | grep CVS`
cd $TMP && rm -fr `find . -type f | grep .cvsignore`

echo "Compressing files to $bin_name..."
cd $TMP && tar -cf $bin_name".tar" * && bzip2 -9 $bin_name".tar"
cd $TMP && mv $bin_name* $CREATED 

echo "Done!"
sleep 5
