#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <time.h>

#define FALSE 0
#define TRUE 1

int main(int argc, char **argv)
{
  struct addrinfo hints;
  struct addrinfo * res;

  memset(&hints, 0, sizeof(struct addrinfo));

  int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

  if ( rc != 0 )
	return -1;
  

  int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
  {
    return -1;
  }

  freeaddrinfo(res);
  
  char buffer[40];
  char respuesta[40];
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  struct sockaddr client_addr;
  socklen_t client_len = sizeof(struct sockaddr);
  int cont = TRUE;

  while (cont) {
  }
  
  return 0;
}
