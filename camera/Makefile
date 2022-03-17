build-camerad:
	clang++ --std=c++17 -I ./third-party/opencl/include -L ./third-party/opencl/include -I ./cereal -L ./cereal  camerad/main.cpp -o camera -l messaging -l zmq

build-mcud:
	clang++ -std=c++17 -I ./cereal -L ./cereal -lusb-1.0 mcud/main.cpp -o mcu -l messaging -l zmq

build-controllerd:
	clang++ --std=c++17 -I ./cereal -L /data/lte-rover/camera/cereal controllerd/main.cpp -o controller -l messaging -l zmq
