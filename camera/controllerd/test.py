import socket

if __name__ == "__main__":
  print("starting udp client")
  udp_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
  
