const express = require("express")
const http = require("http")
const WebSocket= require("ws")
const path = require("path")
const fs = require("fs")
const net = require("net")

// setup http server
const app = express()
const server = http.createServer(app)

// set up websocket
const wss = new WebSocket.Server({ server })
let client
wss.on("connection", (ws) => {
  console.log("new websocket connection")
  client = ws

  fs.open("./video-bridge.h264", fs.constants.O_RDONLY | fs.constants.O_NONBLOCK, (err, fd) => {
    console.log("named pipe open")
    if (err) {
      console.log("error setting up pipe")
    }
    const pipe = new net.Socket({fd})
    pipe.on('data', (data) => {
      console.log("new data")
      ws.send(data)
    })
  })
})

// host website
app.use(express.static("static"))
app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "/static/index.html"))
})

server.listen(8080, () => {
  console.log("websocket server listening", server.address().port)
})
