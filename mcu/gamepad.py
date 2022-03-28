import hid

gamepad = hid.device()
gamepad.open(1356, 2508)

gamepad.set_nonblocking(True)

while True:
  report = gamepad.read(64)
  if report:
    print(report)
