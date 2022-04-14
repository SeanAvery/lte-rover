// setup websocket
// handle websocket
const ws = new WebSocket("ws://localhost:8080")

ws.onopen = () => {
  console.log("websocket connected")
}

ws.onmessage = (evt) => {
  console.log("received msg")
}

// render loop
let frame
const render = () => {
  console.log("render")
}

window.setInterval(render, 10)
