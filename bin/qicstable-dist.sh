#!/bin/bash

# This is the packaging script for QicsTable.  It can be used to package
# five different things.
#
# qicstable-dist
# --------------
#
# The following four options will check out source from SVN and make
# the appropriate package:
#
# unix              Unix commercial version
# win               Windows commercial version
# gpl               GPL version (Unix)
# eval              commercial eval (Unix, Unix no-license)
#


#
# Setup vars
#
proj=qicstable
tdir=`pwd`"/../.."


#get system info
ker_name=`uname -s`
platform=`uname -i`
machine=`uname -m | tr [:upper:] [:lower:]`
gcc_v=`gcc -v 2>&1 | grep version | cut -d " " -f 3`
qit_v=`cat ../VERSION`
qt_v=`$QTDIR/bin/qmake -v 2>&1 | grep -m1 -o -P "\d\.\d\.\d"`

if [ $QICSTABLE_SUFFIX ]
then
    version=$QICSTABLE_SUFFIX
else
    version=$qit_v
fi


PACKAGE_NAME=""
PACKAGE_TYPE=""
OS_NAME=""
SUFFIX=""


########################################################
##############           MAIN            ###############
########################################################


if [ -z $1 ]
then
    echo "No input parameters"
    exit 1
fi


#######################################################

OS_NAME=$1
echo "OS name: "$OS_NAME

PACKAGE_TYPE=$2
echo "Package name: "$PACKAGE_TYPE

SUFFIX=$3
echo "Suffix: "$SUFFIX

if [ -z $SUFFIX ]
then
    PACKAGE_NAME=${proj}"-"${version}"-"$OS_NAME"-source-"$PACKAGE_TYPE
else
    PACKAGE_NAME=${proj}"-"${version}"-"$OS_NAME"-source-"$PACKAGE_TYPE"-"$SUFFIX
fi
echo "Package name: "$PACKAGE_TYPE

########################################################
echo "Deleting old sources"

cd $tdir
rm -rf package/$proj
mkdir -p package

echo "Old package sources cleared..."

########################################################

#Make package dir and copy qicstable

echo "Preparing working dir..."
cd $tdir
cp -r $tdir/$proj $tdir/package

echo "Changing top directory..."
tdir=$tdir/package
top=$tdir/$proj

cd $top
echo "Removing SVN entries"
find . -name .svn -print | xargs rm -rf
#find . -name .cvsignore -print | xargs rm -f
rm -rf tests
rm -rf examples/tabledemo


if [ $PACKAGE_TYPE = "commercial" ]
then
    if [ -z $SUFFIX ]
    then
        echo "cp -r $tdir/../pyqicstable $top/"
    else
        rm -rf designer
    fi
else
    rm -rf designer
fi


#
# Remove unneeded license/install files
#
cd $top
mkdir tmpinst
mv INSTALL* tmpinst

if [ $PACKAGE_TYPE = "gpl" ]
then
   cp tmpinst/INSTALL.GPL INSTALL
elif [ $PACKAGE_TYPE = "commercial" ]
then
    if [ -z $SUFFIX ]
    then
        cp tmpinst/INSTALL INSTALL
    else
        if [ $OS_NAME = "win" ]
        then
           cp tmpinst/INSTALL.WINEVAL INSTALL
        else
           cp tmpinst/INSTALL.UNIXEVAL INSTALL
        fi
    fi 
fi
rm -rf tmpinst

cd $top
mkdir tmpinst
mv LICENSE* tmpinst

if [ $PACKAGE_TYPE = "gpl" ]
then
   cp tmpinst/LICENSE.GPL LICENSE
elif [ $PACKAGE_TYPE = "commercial" ]
then
    if [ -z $SUFFIX ]
    then
        cp tmpinst/LICENSE LICENSE
    else
        cp tmpinst/LICENSE.EVAL LICENSE
    fi
fi

rm -rf tmpinst

rm -f qicstable_wineval.pro




#
# Preprocess files (remove commercial features, or maybe just the tags)
# Get rid of unneeded files
#
if [ $PACKAGE_TYPE = "gpl" ]
then
   script="remove_comm_features"

   cd $top/src
   rm -f QicsTablePrint.*
   rm -f QicsPrintGrid.*

   cd $top/include
   rm -f QicsTablePrint.*
   rm -f QicsPrintGrid.*

   cd $top/addons
   rm -r ./printing

   cd $top/addons
   rm -r ./pyqicstable

   cd $top/addons
   rm -r ./kdchart

   cd $top/demos
   rm -r ./kdchart

   cd $top/demos
   rm -r ./kdchart2

   cd $top/demos/advanced_spreadsheet
   rm -r ./kdchartobject.h
   rm -r ./kdchartobject.cpp
elif [ $PACKAGE_TYPE = "commercial" ]
then
    if [ -z $SUFFIX ]
    then
        script="remove_comm_tags"
    else
       script="remove_comm_features"
       $top/bin/process $top/bin/$script $top/qicstable.pro
       script="remove_comm_tags"
    fi
fi

cd $top
find -name '*.h' -exec $top/bin/process $top/bin/$script {} \; -print 2> /dev/null
find -name '*.cpp' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null
find -name '*.pri' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null
find -name '*.pro' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null



#cd $top/src
#for f in `ls *.cpp *.h *.pro *.pri 2> /dev/null`
#do
#    $top/bin/process $top/bin/$script $f
    #$top/bin/$script < $f > $f.new
    #rm -f $f
    #mv $f.new $f
#done

#cd $top/include
#for f in `ls *.cpp *.h *.pro *.pri 2> /dev/null`
#do
#    $top/bin/process $top/bin/$script $f
    #$top/bin/$script < $f > $f.new
    #rm -f $f
    #mv $f.new $f
#done

#cd $top
#for f in `ls *.cpp *.h *.pro *.pri 2> /dev/null`
#do
#    $top/bin/process $top/bin/$script $f
    #$top/bin/$script < $f > $f.new
    #rm -f $f
    #mv $f.new $f
#done

#
# Preprocess files to remove windows-specific stuff
# if we're doing non-Windows versions
#
if [ $OS_NAME != "win" ]
then
   cd $top/src

   for f in `ls *.cpp *.h *.pro *.pri 2> /dev/null `
   do
       $top/bin/remove_win_features < $f > $f.new
       rm -f $f
       mv $f.new $f
   done
fi

#
# Put the correct copyright notice in each source file
#

# first the source files
cd $top/src

for  f in `ls *.cpp *.h 2> /dev/null`
do
    $top/bin/remove_default_copyright < $f > $f.new
    rm -f $f

    if [ $PACKAGE_TYPE = "gpl" ]
    then
       hdr=$top/config/gpl_source_header.txt
    else
       hdr=$top/config/comm_source_header.txt
    fi

    cat $hdr > $f
    cat $f.new >> $f
    rm -f $f.new
done

# now the qmake files
cd $top/src
for f in `ls *.pro *.pri 2> /dev/null`
do
    $top/bin/remove_default_copyright < $f > $f.new
    rm -f $f

    if [ $PACKAGE_TYPE = "gpl" ]
    then
       hdr=$top/config/gpl_pro_header.txt
    else
       hdr=$top/config/comm_pro_header.txt
    fi

    cat $hdr > $f
    cat $f.new >> $f
    rm -f $f.new
done


#
# Preprocess qmakefile to get rid of eval and gpl stuff
#

cd $top
if [ $PACKAGE_TYPE = "commercial" ]
then
    if [ -z $SUFFIX ]
    then
        rm -rf $top/src/evaldialog
        script="remove_eval_features"
    else
        if [ $OS_NAME = "win" ]
        then
            script="remove_eval_tags"
        else
            rm -rf $top/src/evaldialog
            script="remove_eval_features"
        fi
    fi
else
   rm -rf $top/src/evaldialog
   script="remove_eval_features"
fi

#$script < qicstable.pro > qicstable.pro.new
#rm -f qicstable.pro
#mv qicstable.pro.new qicstable.pro

cd $top
find -name '*.h' -exec $top/bin/process $top/bin/$script {} \; -print 2> /dev/null
find -name '*.cpp' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null
find -name '*.pri' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null
find -name '*.pro' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null


if [ $PACKAGE_TYPE = "gpl" ]
then
   script="remove_gpl_tags"
else
   script="remove_gpl_features"
fi

#$script < qicstable.pro > qicstable.pro.new
#rm -f qicstable.pro
#mv qicstable.pro.new qicstable.pro

cd $top
find -name '*.h' -exec $top/bin/process $top/bin/$script {} \; -print 2> /dev/null
find -name '*.cpp' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null
find -name '*.pri' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null
find -name '*.pro' -exec $top/bin/process $top/bin/$script {} \;  -print 2> /dev/null

# Generate PyQicsTable documentation
echo Generate PyQicsTable documentation ...

if [[ $OS_NAME = "win" || $OS_NAME = "unix" ]]
then
   cd $top/addons/pyqicstable/doc
   db2html -o html -h PyQicsTable.sgml
   rm -rf PyQicsTable
   rm -f PyQicsTable.sgml
   rm -f *.dsl
   rm -f *.local
fi

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
# Get rid of unneeded directories
#
cd $top
rm -rf bin config Include adapter pack  plugins VERSION lib
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
# Make unix tar file
#
if [ $OS_NAME != "win" ]
then
   cd $tdir
   mv $proj $PACKAGE_NAME
   tar cvf ${PACKAGE_NAME}.tar $PACKAGE_NAME
   gzip ${PACKAGE_NAME}.tar
   mv -f $PACKAGE_NAME $proj
fi

#
# Make Windows zip file
#
if [ $OS_NAME = "win" ]
then
    cd $tdir
    mv $proj $PACKAGE_NAME
    zip -r ${PACKAGE_NAME}.zip $PACKAGE_NAME
    mv $PACKAGE_NAME $proj
fi


#
# remove working dir

echo remove working dir .........
cd $top
cd ..
rm -rf $proj



