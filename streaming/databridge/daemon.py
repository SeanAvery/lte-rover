from flask import Flask
from flask_socketio import SocketIO

# web server runs on port 5000
app = Flask(__name__)
socket = SocketIO(app, async_mode=None)

if __name__ == "__main__":
  socket.run(app, debug=True)
