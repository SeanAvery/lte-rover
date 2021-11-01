build:
	/opt/homebrew/share/android-ndk/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android28-clang++ -Wmacro-redefined -Wc99-designator -static-libstdc++ camerad/main.cpp -o camera

push:
	adb push camera /data
 
