build-camerad:
	clang++ --std=c++17 camerad/main.cpp -o camera

build-mcud:
	clang++ -std=c++17 -I ./cereal -L ./cereal -lusb-1.0 mcud/main.cpp -o mcu -l messaging -l zmq

build-controllerd:
	clang++ --std=c++17 -I ./cereal -L /data/lte-rover/camera/cereal controllerd/main.cpp -o controller -l messaging -l zmq
