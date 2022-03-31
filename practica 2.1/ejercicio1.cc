#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char **argv)
{
  struct addrinfo hints;
  struct addrinfo * res;

  memset(&hints, 0, sizeof(struct addrinfo));

  int rc = getaddrinfo(argv[1], nullptr , &hints, &res);

  if ( rc != 0 )
  {
    std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
    return -1;
  }
  socklen_t client_len = sizeof(struct sockaddr);
  char host[NI_MAXHOST];

  while (res != NULL)
  {
    getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, NULL, NI_MAXSERV, NI_NUMERICHOST);

    std::cout << "IP: " << host << " FAMILY: " << res->ai_family << " SOCKTYPE: " << res->ai_socktype << std::endl;
    res = res->ai_next;
  }
  freeaddrinfo(res);
  return 0;
}
