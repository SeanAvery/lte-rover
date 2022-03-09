#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define UDP_PORT 900
#define UDP_HOST "192.168.1.9"
#define BUF_MAX 1024

int main()
{
  std::cout << "starting controller daemon" << std::endl;
  int sockfd;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  std::cout << "AF_INET" << AF_INET << std::endl;
  std::cout << "SOCK_DGRAM" << SOCK_DGRAM << std::endl;
  std::cout << "sockfd" << sockfd << std::endl;
  struct sockaddr_in serv, client;
  serv.sin_family = AF_INET;
  // serv.sin_addr.s_addr = htonl(INADDR_ANY);
  serv.sin_addr.s_addr = inet_addr(UDP_HOST);
  serv.sin_port = htons(UDP_PORT);
  int err = bind(sockfd, (const struct sockaddr *)&serv, sizeof(serv));
  if (err != 0)
  {
    std::cout << "error binding socket: " << err << std::endl;
  }
  char buffer[BUF_MAX];
  socklen_t len = sizeof(client);
  while (true)
  {
    usleep(1000000);
    // check socket for msg
    std::cout << "checking socket" << std::endl;
    ssize_t n = recvfrom(sockfd, (char *)buffer, BUF_MAX, MSG_WAITALL, (struct sockaddr *) &client, &len);
    
  }
}
