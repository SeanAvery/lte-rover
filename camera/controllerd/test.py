import socket
import time

server_addr = "192.168.1.9"
server_port = 900
test_msg = bytes("s00110#", 'utf-8')

if __name__ == "__main__":
  print("starting udp client")
  udp_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
  
  while True:
    time.sleep(1)
    print("sending udp packet")
    udp_socket.sendto(test_msg, (server_addr, server_port))

  
  
