let gamepad;

window.addEventListener("gamepadconnected", e => {
  console.log("gamepad connected")
  gamepad = navigator.getGamepads()[0]
  console.log(gamepad)

})

window.addEventListener("gamepaddisconnected", e=> {
  console.log("gamepad disconnected")
})


const gameLoop = () => {
  if (gamepad) {
  console.log("yooo")
    for (const button in gamepad.buttons) {
      // console.log(gamepad.buttons[button])
      if (gamepad.buttons[button].touched) {
        console.log(button)
      }
    }
  }
}

window.setInterval(gameLoop, 1000)
