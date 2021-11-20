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
  }
  
}
