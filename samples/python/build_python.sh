#!/usr/bin/bash

set -x
set -e

### Checking that the smartIdEngine library path is passed as an argument

if [[ $# -ne 2 ]]; then
  echo "Wrong number of parameters: "
  echo "Usage: $0 <PATH_TO_SMARTIDENGINE_LIBRARY_DIRECTORY> <VERSION_OF_PYTHON>"
  exit 1
fi

SMARTID_LIBRARY_PATH="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"
BUILD_DIR="$(pwd)"
Python_ADDITIONAL_VERSIONS=$2


### Unpacking, building, and installing (locally) SWIG 3.0.12

cd $BUILD_DIR
tar -xf swig-3.0.12.tar.gz
cd swig-3.0.12
./configure --without-pcre --prefix=$BUILD_DIR/swig
make
make install
cd ../..

### Making PHP module

mkdir $BUILD_DIR/build
cd $BUILD_DIR/build
cmake .. -DSWIG_EXECUTABLE=$BUILD_DIR/swig/bin/swig \
         -DSWIG_DIR=$BUILD_DIR/swig/share/swig/3.0.12 \
         -DSMARTID_LIBRARY_PATH=$SMARTID_LIBRARY_PATH \
         -DPython_ADDITIONAL_VERSIONS=$Python_ADDITIONAL_VERSIONS 
make install
make test