#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

/*
./tcp 0.0.0.0 7777
*/

class ConexionTCP
{
public:
  ConexionTCP(int sd): sd_client(sd){}

  void haz_conexion(){
    // ---------------------------------------------------------------------- //
    // GESTION DE LA CONEXION CLIENTE //
    // ---------------------------------------------------------------------- //

    bool detector = false;
    while(!detector)
    {
      char buffer[80];

      ssize_t bytes = recv(sd_client, buffer, sizeof(char)*79, 0);
      buffer[80] = '\0';
      std::cout << "Cliente: " << sd_client << " ID: " << std::this_thread::get_id() << "\n";
      if ( bytes <= 0 )
      {
        std::cout << "Conexión terminada" << std::endl;
        detector = true;
        continue;
      }

      send(sd_client, (void *) buffer, bytes, 0);
    }
    close(sd_client);
  }
private:
  int sd_client;
};

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

if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
{
std::cerr << "bind: " << std::endl;
return -1;
}

freeaddrinfo(res);

// ---------------------------------------------------------------------- //
// PUBLICAR EL SERVIDOR (LISTEN) //
// ---------------------------------------------------------------------- //
listen(sd, 2);

while(true)
{
  // ---------------------------------------------------------------------- //
  // GESTION DE LAS CONEXIONES AL SERVIDOR //
  // ---------------------------------------------------------------------- //
  struct sockaddr client_addr;
  socklen_t client_len = sizeof(struct sockaddr);

  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  int sd_client = accept(sd, &client_addr, &client_len);

  getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
  NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

  std::cout << "CONEXION DESDE IP: " << host << " PUERTO: " << service << " Cliente " << sd_client
  << std::endl;

  //Creacion de la clase con el socket del cliente
  ConexionTCP* tcp = new ConexionTCP(sd_client);

  //Nuevo thread para tratar al cliente
  std::thread conexionThread(&ConexionTCP::haz_conexion, tcp);
  conexionThread.detach();
}
return 0;
}
