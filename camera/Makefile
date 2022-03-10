build-camerad:
	/opt/homebrew/share/android-ndk/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android28-clang++ -Wmacro-redefined -Wc99-designator -std=c++17 -static-libstdc++ camerad/main.cpp -o camera

push-camerad:
	scp camera root@$(IP):/data

up-camerad:
	/opt/homebrew/share/android-ndk/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android28-clang++ -Wmacro-redefined -Wc99-designator -std=c++17 -static-libstdc++ camerad/main.cpp -o camera
	scp camera root@$(IP):/data

build-mcud:
	clang++ -std=c++17 $(pkg-config --cflags libusb-1.0) mcud/main.cpp -lusb-1.0 -o mcu $(pkg-config --cflags libusb-1.0)

push-mcud:
	scp mcu root@$(IP):/data

push-joypadd:
	scp joypadd/server.py root@$(IP):/data

build-controllerd:
	clang++ --std=c++17 -I ./cereal controllerd/main.cpp -o controller
