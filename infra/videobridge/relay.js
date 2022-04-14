const express = require("express")
const http = require("http")
const WebSocket= require("ws")
const path = require("path")

// setup http server
const app = express()
const server = http.createServer(app)

// set up websocket
const wss = new WebSocket.Server({ server })
let client
wss.on("connection", (ws) => {
  console.log("new websocket connection")
  client = ws
})

// host website
app.use(express.static("static"))
app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "/static/index.html"))
})

server.listen(8080, () => {
  console.log("websocket server listening", server.address().port)
})
