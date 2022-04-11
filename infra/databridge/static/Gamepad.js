
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
      throttle = throttle - decelerate
      console.log("decelerate: ", decelerate)
    }
    const accelerate = gamepad.buttons[7].value
    if (accelerate > 0) {
      throttle = throttle + accelerate
      console.log("accelerate: ", accelerate)
    }

    // steering = gamepd.axis[0] a
    console.log('axis', gamepad.axes)
    steering = Math.round(90 + gamepad.axes[0]*20)
    
    // console.log("steering: ", steering)

    // format msg
    let steeringMsg = "000" + steering
    steeringMsg = steeringMsg.substr(steeringMsg.length - 5, steeringMsg.length)
    steeringMsg = "s" + steeringMsg + "#"
    console.log("steeringMsg", steeringMsg)
    ws.send(steeringMsg)

    console.log("throttle:", throttle)
    let throttleMsg = "000" + Math.round(throttle)
    throttleMsg = throttleMsg.substr(throttleMsg.length - 5, throttleMsg.length)
    throttleMsg = "t" + throttleMsg + "#"
    console.log("throttleMsg:", throttleMsg)

  }
}

window.setInterval(gameLoop, 100)
