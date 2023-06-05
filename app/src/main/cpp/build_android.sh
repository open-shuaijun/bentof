export NDK=/home/easytouch/android/ndk/android-ndk-r16b
export MINSDKVERSION=17
export ABI=armeabi-v7a
export INSTALL_PERFIX=./lib

mkdir android
cd android
cmake .. -DBUILD_SHARED_LIBS=ON -DBUILD_SHARED=ON -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_NATIVE_API_LEVEL=$MINSDKVERSION ..  -DCMAKE_INSTALL_PREFIX=$INSTALL_PERFIX
make -j8
make install

