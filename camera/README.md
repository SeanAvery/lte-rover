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
1. Daemon udp server receives packets from video bridge
2. Publishes `controls` packets over zmq

```
make build-controllerd
./controller
```

### camerad

```
may just run straight op for this one
```

## Communication

checkout out openpilot's cereal library

