build:
	docker build ./ -t video-bridge

run:
	docker run -it --network="host" -p 8000:8000 -p 8088:8088 -p 8089:8089 video-bridge
