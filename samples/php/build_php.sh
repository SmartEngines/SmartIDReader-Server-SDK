#!/usr/bin/bash

set -x
set -e

### Checking that the smartIdEngine library path is passed as an argument

if [[ $# -ne 2 ]]; then
  echo "Wrong number of parameters: "
  echo "Usage: $0 <PATH_TO_SMARTIDENGINE_LIBRARY_DIRECTORY> <PATH_TO_YOUR_php-config_BINARY>"
  exit 1
fi

SMARTID_LIBRARY_PATH="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"
BUILD_DIR="$(pwd)"
PHP_CONFIG_PATH="$2"
PHP_EXTENSION_PATH="$2 --extension-dir"

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
         -DPHP_CONFIG_PATH=$PHP_CONFIG_PATH
make install
# sudo cp $BUILD_DIR/../bin/phpSmartIdEngine.so $PHP_EXTENSION_PATH
make test