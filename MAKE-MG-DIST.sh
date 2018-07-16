#! /bin/sh
### ====================================================================
### Make an mg 1.3x distribution file containing binaries and manual
### pages for a specified architecture.
###
### Usage:
###	./MAKE-MG-DIST.sh tar-file-name [ otherfiles ]
###
### [30-Oct-2004] -- update for version mg-1.3.64x and location
###		     independence
### [17-May-1997]
### ====================================================================

prefix=${prefix-/usr/local}

if test $# -lt 1
then
    echo Usage: $0 tar-file-name [ otherfiles ]
    exit 1
fi

TOP=`dirname $0`/mg-1.3.64x

tarfile=$1
shift

tar cf $tarfile \
	-C $TOP "$@" \
	-C $prefix `cat $TOP/MAKE-MG-DIST.bin-files` \
	-C $prefix `cat $TOP/MAKE-MG-DIST.man-files`
