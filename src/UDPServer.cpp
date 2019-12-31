#include "UDPServer.h"
#include <stdio.h>
#include <cstring>

UDPServer::UDPServer(uint16 port)
{
    printf("Creation of the UDP Server...\n");

    // Non blocking socket
    this->udpSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    fcntl(this->udpSocket, F_SETFL, fcntl(this->udpSocket, F_GETFL) | O_NONBLOCK);

    // Address
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&this->address.sin_zero, 0, 8);

    // Binding
    if (bind(this->udpSocket, (SOCKADDR*) &this->address, sizeof(this->address)) == -1)
    {
        printf("Failure to create the UDP Server!\n");
        return;
    }

    printf("UDP Server created successfully!\n");
}

std::string UDPServer::receive(std::string *senderIP, uint16 *senderPort)
{
    SOCKADDR_IN senderAddress;
    socklen_t senderAddressSize = 0;
    char message[MESSAGE_MAX_SIZE] = { 0 };

    // Reception of a potential message
    senderAddressSize = sizeof(senderAddress);

    if (recvfrom(this->udpSocket, message, MESSAGE_MAX_SIZE, 0, (struct sockaddr*) &senderAddress, &senderAddressSize) != -1)
    {
        *senderIP = inet_ntoa(senderAddress.sin_addr);
        *senderPort = ntohs(senderAddress.sin_port);

        return message;
    }

    return "";
}

void UDPServer::send(std::string message, std::string receiverIP, uint16 receiverPort)
{
    // Adresse of a receiver
    SOCKADDR_IN receiverAddress;
    receiverAddress.sin_family = AF_INET;
    receiverAddress.sin_port = htons(receiverPort);
    memset(&receiverAddress.sin_zero, 0, 8);

    hostent *receiverInfos = gethostbyname(receiverIP.c_str());
    receiverAddress.sin_addr.s_addr = ((in_addr*) receiverInfos->h_addr)->s_addr;

    if (receiverAddress.sin_addr.s_addr == 0)
        receiverAddress.sin_addr.s_addr = inet_addr(receiverIP.c_str());

    if (sendto(this->udpSocket, message.c_str(), message.length() + 1, 0, (sockaddr*) &receiverAddress, sizeof(receiverAddress)) == -1)
        printf("Failure to send the message!\n");
}
