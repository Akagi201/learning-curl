#/bin/sh

if test -d build ;
then rm -rf build
fi
mkdir build
cp main.c build/

cd build
# cmake ..
cmake .. && make
