let gamepad;

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
      if (gamepad.buttons[button].value > 0) {
        console.log(button)
      }
    }  
  }
}

window.setInterval(gameLoop, 1000)
