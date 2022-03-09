let gamepad;

const leftTrigger = 6;
const rightTrigger = 7;

window.addEventListener("gamepadconnected", e => {
  console.log("gamepad connected")
  gamepad = navigator.getGamepads()[0]
  console.log("gamepads", navigator.getGamepads())
  console.log(gamepad)

})

window.addEventListener("gamepaddisconnected", e=> {
  console.log("gamepad disconnected")
})


const gameLoop = () => {
  gamepad = navigator.getGamepads()[0]
  if (gamepad) {
    for (const button in gamepad.buttons) {
      const value = gamepad.buttons[button].value
      if (value > 0) {
        if (button == leftTrigger) {
          console.log("leftTrigger", value)
        }
        if (button == rightTrigger) {
          console.log("rightTrigger", value)
        }
      }
    }  
  }
}

window.setInterval(gameLoop, 1000)
