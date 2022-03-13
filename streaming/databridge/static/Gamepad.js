
let gamepad;

// handle gamepad
window.addEventListener("gamepadconnected", e => {
  console.log("gamepad connected")
  gamepad = navigator.getGamepads()[0]
  console.log("gamepads", navigator.getGamepads())
  console.log(gamepad)

})

window.addEventListener("gamepaddisconnected", e=> {
  console.log("gamepad disconnected")
})


// handle websocket
const ws = new WebSocket("ws://localhost:8080")

ws.onopen = () => {
  console.log("websocket connected")
}

// controls state
let throttle = 90
const minThrottle = 90
const maxThrottle = 100

let steering = 90
const minSteering = 70
const maxSteering = 110

const gameLoop = () => {
  gamepad = navigator.getGamepads()[0]
  if (gamepad) {
    const decelerate = gamepad.buttons[6].value
    if (decelerate > 0) {
      console.log("decelerate: ", decelerate)
    }
    const accelerate = gamepad.buttons[7].value
    if (accelerate > 0) {
      console.log("accelerate: ", accelerate)
    }

    // steering = gamepd.axis[0] a
    console.log('axis', gamepad.axes)
    steering = Math.round(90 + gamepad.axes[0]*20)
    console.log("steering: ", steering)
    for (const button in gamepad.buttons) {
      const value = gamepad.buttons[button].value
      // console.log(value)
      if (value > 0) {
        console.log("hit: ", button)
      }
    }  
  }
}

window.setInterval(gameLoop, 1000)
