// setup websocket
// handle websocket
const ws = new WebSocket("ws://localhost:8080")

ws.onopen = () => {
  console.log("websocket connected")
}


// render loop
const render = () => {
  console.log("render")
}

window.setInterval(render, 10)
