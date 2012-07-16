#!/bin/sh

mkdir build
cd build
cmake ../../../src -DENABLE_TESTS=True
make
make test
#rm -r build
cp ../../../bin/* .


