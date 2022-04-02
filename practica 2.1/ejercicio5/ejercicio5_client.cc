#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

/*
./tcp 0.0.0.0 7777
*/

int main(int argc, char **argv)
{
struct addrinfo hints;
struct addrinfo * res;

// ---------------------------------------------------------------------- //
// INICIALIZACIÓN SOCKET & BIND //
// ---------------------------------------------------------------------- //

memset(&hints, 0, sizeof(struct addrinfo));

hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;

int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

if ( rc != 0 )
{
std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
return -1;
}

// res contiene la representación como sockaddr de dirección + puerto

int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

//freeaddrinfo(res);

// ---------------------------------------------------------------------- //
// GESTION DE LAS CONEXIONES AL SERVIDOR //
// ---------------------------------------------------------------------- //
struct sockaddr serv_addr;
socklen_t serv_len = sizeof(struct sockaddr);

char host[NI_MAXHOST];
char service[NI_MAXSERV];

int sd_server = connect(sd, res->ai_addr, res->ai_addrlen);

if(sd_server < 0)
{
  std::cout << "Error al conectar con el servidor" << std::endl;
  return -1;
}

getnameinfo(&serv_addr, serv_len, host, NI_MAXHOST, service,
NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

// ---------------------------------------------------------------------- //
// GESTION DE LA CONEXION CLIENTE //
// ---------------------------------------------------------------------- //
char buffer[80];
while(buffer[0] != 'Q')
{
  std::cin >> buffer;
  buffer[80]='\0';
  send(sd, (void *) buffer, sizeof(char)*79, 0);

  if (buffer[0] == 'Q')
  {
  continue;
  }
  ssize_t bytes = recv(sd, (void *) buffer, sizeof(char)*79, 0);
  std::cout << buffer << std::endl;
}
return 0;
}
