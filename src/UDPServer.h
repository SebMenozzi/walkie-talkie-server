#ifndef UDPSERVER_H_INCLUDED
#define UDPSERVER_H_INCLUDED

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <string>
#include "types.h"

typedef sockaddr_in SOCKADDR_IN;
typedef sockaddr SOCKADDR;
typedef int SOCKET;
typedef socklen_t SOCKLEN_T;

#define MESSAGE_MAX_SIZE 65507

class UDPServer
{
    private:
        SOCKET udpSocket;
        SOCKADDR_IN address;

    public:
        UDPServer(uint16 port);
        std::string receive(std::string *senderIP, uint16 *senderPort);
        void send(std::string message, std::string receiverIP, uint16 receiverPort);
};

#endif // UDPSERVER_H_INCLUDED
