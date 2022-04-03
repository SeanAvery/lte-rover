build-db:
	docker build -t data-bridge ./databridge

run-db:
	docker run  --name=data-bridge -p 900:900 -p 8080:8080 data-bridge

build-vb:
	docker build -t video-bridge ./videobridge

run-vb:
	docker run -it -p 8000:8000 -p 8088:8088 -p 8089:8089 video-bridge
