# build scripts 
build-camerad:
	clang++ --std=c++17 -I ./third-party/opencl/include -L ./third-party/opencl/include -I ./cereal -L ./cereal  camerad/main.cpp -o camera -l messaging -l zmq

build-mcud:
	clang++ -std=c++17 -I ./third-party/ftdi -L ./third-part/ftdi -I ./cereal -L ./cereal -lusb-1.0 mcud/main.cpp -o mcu -l messaging -l zmq

build-controllerd:
	clang++ --std=c++17 -I ./cereal -L /data/lte-rover/camera/cereal controllerd/main.cpp -o controller -l messaging -l zmq

build-streamerd:
	clang++ --std=c++17 -I ./cereal -L ./cereal -I ./third-party/openmax/include streamerd/main.cpp -I ./common -I./streamerd/encoder/incude -L /system/vendor/lib64 -L /system/comma/usr/lib -L /user/local/lib -L /usr/lib -I third-party/opencl/include -o streamer -l messaging -l zmq -l OmxCore -l OmxVenc -l gsl -l CB -l OpenCL

build-mcud-new:
	cd /data/libftdi/build/ && make && cd src && cp lib* /data/lte-rover/camera/third-party/ftdi/
	clang++ -std=c++17 -I ./third-party/ftdi/ mcud-new/main.cpp -L ./third-party/ftdi/ -o mcu-new -l ftdi1

# tools

start-camerad:
	chmod +x ./scripts/start_camerad_wide.sh
	./scripts/start_camerad_wide.sh
