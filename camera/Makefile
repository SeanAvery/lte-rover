build-camerad:
	/opt/homebrew/share/android-ndk/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android28-clang++ -Wmacro-redefined -Wc99-designator -static-libstdc++ camerad/main.cpp -o camera

push-camerad:
	adb push camera /data

build-mcud:
	/opt/homebrew/share/android-ndk/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android28-clang++ -Wmacro-redefined -Wc99-designator -static-libstdc++ -I/opt/homebrew/Cellar/libusb/1.0.24/include/ mcud/main.cpp -o mcu

push-mcud:
	adb push mcu /data


