# camera

This is basically turning into a mini-openpilot taking qualcomm camera code, usb communication stuff, and video encoding module out. Just need to add video streaming code (rtp or ideally webrtc/janus).

## Processes

### mcud

1. Libusb driver for mcu (ftdi)
2. Subscriber to `controls` events over zmq
3. Writes `controls` packets over usb
4. Reads `mcu_state` packets over usb

```
make build-mcud
./mcu

```

### controllerd
1. Daemon udp server receives packets from data-bridge
2. Publishes `controls` packets over zmq

```
make build-controllerd
./controller
```

### camerad

```
may just run straight op for this one

./camerad # from openpilot build
```

### streamerd
1. Accepts frame packets over VisionIPC
2. Encodes to h64 using OpenMax
3. Streams over RTP to video-bridge

```
make build-streamerd
./streamer
```


## Communication

### Wireless

Linux socket. Udp on wifi for now. 

Todo: tcp over lte

### Interprocess

checkout out openpilot's cereal library


