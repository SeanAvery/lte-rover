import socket
import time
from flask import Flask, send_from_directory
from flask_socketio import SocketIO

# web server runs on port 5000
app_path = "./static"
app = Flask(__name__, static_folder=app_path)

# setup ws <-> udp socket bridge
ws_socket = SocketIO(app, async_mode=None)
udp_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# host webpage
@app.route("/")
def serve():
  return send_from_directory(app.static_folder, "index.html")

@ws_socket.on('connect')
def connect():
  print("connected to websocket")

# @socket_.on('controller', namespace='/ctrls')
if __name__ == "__main__":
  ws_socket.run(app, debug=True)
