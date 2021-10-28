TARGET=aarch64-linux-android
PREFIX=/usr/local/$TARGET

cd binutils && mkdir build && cd build

../binutils-2.27/configure --target=$TARGET --prefix=$PREFIX --enable-werror=no

make -j8 && sudo make install

