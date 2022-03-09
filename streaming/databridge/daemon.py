import socket
import time
from flask import Flask
from flask_socketio import SocketIO

# web server runs on port 5000
app = Flask(__name__)

# setup ws <-> udp socket bridge
ws_socket = SocketIO(app, async_mode=None)
udp_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

if __name__ == "__main__":
  ws_socket.run(app, debug=True)
