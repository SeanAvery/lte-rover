const express = require("express")
const http = require("http")
const WebSocket= require("ws")
const path = require("path")
const dgram = require("dgram")

// setup http server
const app = express()
const server = http.createServer(app)

// setup udp bridge
const udpHost = "192.168.1.9"
const udpPort = 900
const udpClient = dgram.createSocket("udp4")

// setup websocket
const wss = new WebSocket.Server({ server })

wss.on("connection", (ws) => {
  console.log("new websocket connection")
})

// host website
app.use(express.static("static"))
app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "/static/index.html"))
})

server.listen(8080, () => {
  console.log("websocket server listening", server.address().port)
})
