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

// ---------------------------------------------------------------------- //
// INICIALIZACIÓN SOCKET//
// ---------------------------------------------------------------------- //

memset(&hints, 0, sizeof(struct addrinfo));

hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_DGRAM;

int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

if ( rc != 0 )
{
std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
return -1;
}

// res contiene la representación como sockaddr de dirección + puerto

int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

if(sd < 0)
{
  std::cerr << "socket error: \n";
  return -1;

}


// ---------------------------------------------------------------------- //
// RECEPCIÓN MENSAJE DE CLIENTE //
// ---------------------------------------------------------------------- //
char buffer[30];
char host[NI_MAXHOST];
char service[NI_MAXSERV];

struct sockaddr client_addr;
socklen_t client_len = sizeof(struct sockaddr);


// ---------------------------------------------------------------------- //
// RESPUESTA AL CLIENTE //
// ---------------------------------------------------------------------- //
sendto(sd, argv[3], strlen(argv[3])+1, 0, res->ai_addr, res->ai_addrlen);

ssize_t bytes = recvfrom(sd, buffer, 29 * sizeof(char), 0, res->ai_addr, &res->ai_addrlen);

if ( bytes == -1)
{
  std::cerr << "recvfrom: " << std::endl;
  return -1;
}

std::cout << buffer << std::endl;

return 0;
}
