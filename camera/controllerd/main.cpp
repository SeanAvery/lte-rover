#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
  std::cout << "starting controller daemon" << std::endl;
  int sockfd;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  std::cout << "AF_INET" << AF_INET << std::endl;
  std::cout << "SOCK_DGRAM" << SOCK_DGRAM << std::endl;
  std::cout << "sockfd" << sockfd << std::endl;
  struct sockaddr_in serv;
  serv.sin_family = AF_INET;
}
