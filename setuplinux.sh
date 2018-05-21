#!/bin/sh

set -e

cd $(dirname $0)

setupdirOK=1
[ -d src ] || setupdirOK=0
[ -d src/executables ] || setupdirOK=0
[ -d src/libraries ] || setupdirOK=0
[ -d src/shaders ] || setupdirOK=0

if [ x$setupdirOK = x0 ] ; then
  echo " "
  echo "This script ($0) must be called in the project root directory"
  echo "you checked out from the SVN repository."
  echo " "
  exit 1
fi

which cmake > /dev/null || (echo "Bitte erst 'cmake' installieren !!" && exit 1)

echo " "
echo "Please choose your IDE:"
echo " "
echo " (1)  Eclipse with CDT"
echo " (2)  UNIX command line (make)"
echo " "

Gen=""
while [ x"$Gen" = x"" ]
  do
 printf "your choice (1-2): "
 read val
 case "$val" in
  1)
	Folder=BUILD_Eclipse; Gen="Eclipse CDT4 - Unix Makefiles"
	;;
  2)
	Folder=BUILD_CLI; Gen="Unix Makefiles"
	;;
  *)
	echo "Wrong entry - please chose a number between 1 and 2"
	exit 1
	;;
 esac
done
# vorerst nur für Eclipse:

echo "Your choice: $Gen"

mkdir -p "$Folder"
cd "$Folder"
cmake -G"$Gen" ../src/

