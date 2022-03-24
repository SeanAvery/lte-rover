build-camerad:
	clang++ --std=c++17 -I ./third-party/opencl/include -L ./third-party/opencl/include -I ./cereal -L ./cereal  camerad/main.cpp -o camera -l messaging -l zmq

build-mcud:
	clang++ -std=c++17 -I ./cereal -L ./cereal -lusb-1.0 mcud/main.cpp -o mcu -l messaging -l zmq

build-controllerd:
	clang++ --std=c++17 -I ./cereal -L /data/lte-rover/camera/cereal controllerd/main.cpp -o controller -l messaging -l zmq

build-streamerd:
	clang++ --std=c++17 -I ./cereal -L ./cereal -I ./third-party/openmax/include streamerd/main.cpp -I ./common -I./streamerd/encoder/incude -L /system/vendor/lib64 -L /system/comma/usr/lib -L /user/local/lib -L /usr/lib -I third-party/opencl/include -o streamer -l messaging -l zmq -l OmxCore -l OmxVenc -l gsl -l CB -l OpenCL

# clang++ --std=c++17 -I ./cereal -L ./cereal -I ./third-party/openmax/include streamerd/main.cpp -L /system/lib/ -L /system/vendor/lib64 -I ./common -I./streamerd/encoder/incude -I third-party/opencl/include -o streamer -l messaging -l zmq -l OmxCore -l OmxVenc -l gsl -l OpenCL

	
# clang++ --std=c++17 -I /data/arm64/include/gstreamer-1.0 -L /data/arm64/lib -I ./cereal -L ./cereal  -I third-party/opencl/include streamerd/main.cpp -o streamer -l messaging -l zmq
