#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "UDPServer.h"
#include "clock.h"
#include "types.h"
#include "client.h"
#include "utils.h"

int main(int argc, char* argv[])
{
    UDPServer server(2712);

    Clock clock;

    std::vector<Client> clients;

    while (TRUE)
    {
        std::string ip;
        uint16 port;
        std::string receivedMessage = server.receive(&ip, &port);

        if(receivedMessage != "")
        {
            std::string header = decapsulate(&receivedMessage);

            // A new client connects
            if (header == "CONNECTION")
            {
                std::string channel = decapsulate(&receivedMessage);
                std::transform(channel.begin(), channel.end(), channel.begin(), ::tolower);

                bool8 clientExists = FALSE;

                // Search for an existing client
                for(uint32 i = 0; i < clients.size(); ++i)
                {
                    Client &client = clients[i];

                    if (client.ip == ip && client.channel == channel)
                    {
                        // Update the port
                        client.port = port;
                        // Update the time of a last message
                        client.timeLastMessage = clock.time();
                        clientExists = TRUE;
                        break;
                    }
                }

                // The client doesn't exist
                if (clientExists == FALSE)
                {
                    std::cout << ">> Welcome to the client on channel " << channel << " !";
                    std::cout << "(" << ip << ":" << port << ")" << std::endl;

                    // Add the new client
                    Client client;
                    client.channel = channel;
                    client.ip = ip;
                    client.port = port;
                    client.timeLastMessage = clock.time();
                    clients.push_back(client);
                }
            }

            if (header == "NB_CLIENTS")
            {
                std::string channel = decapsulate(&receivedMessage);
                std::transform(channel.begin(), channel.end(), channel.begin(), ::tolower);

                std::string message = "NB_CLIENTS";

                uint32 nb_clients = 0;

                for(uint32 i = 0; i < clients.size(); i++)
                {
                    if (clients[i].channel == channel)
                        nb_clients += 1;
                }

                message += " ";
                message += uint32ToString(nb_clients);

                server.send(message, ip, port);
            }

            if (header == "DECONNEXION")
            {
                // Search for an existing client
                for(uint32 i = 0; i < clients.size(); ++i)
                {
                    Client &client = clients[i];

                    if (client.ip == ip && client.port == port)
                    {
                        std::cout << "Bye! (" << client.ip << ":" << client.port << ")" << std::endl;
                        // Delete the client
                        clients.erase(clients.begin() + i);
                        break;
                    }
                }
            }
        }

        // Delete clients that didn't send anything for more than 20 secs
        for (uint32 i = 0; i < clients.size(); i++)
        {
            if (clients[i].timeLastMessage < clock.time() - 20000)
            {
                std::cout << "Disconnected (" << clients[i].ip << ":" << clients[i].port << ")" << std::endl;

                // Delete the client
                clients.erase(clients.begin() + i);
                i--;
            }
        }
    }

    return EXIT_SUCCESS;
}
