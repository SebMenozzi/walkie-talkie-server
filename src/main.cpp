#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
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

        if(!receivedMessage.empty())
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
                    std::cout << ">> Welcome to the client on channel " << channel << " ! ";
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

                uint32 nb_clients = 0;

                for(uint32 i = 0; i < clients.size(); i++)
                {
                    if (clients[i].channel == channel)
                        nb_clients += 1;
                }

                std::string message = "NB_CLIENTS";
                message += " ";
                message += uint32ToString(nb_clients);

                std::cout << ">> Number of clients on " << channel << " : " << uint32ToString(nb_clients) << std::endl;

                server.send(message, ip, port);
            }

            if (header == "DECONNECTION")
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

            if (header == "AUDIO")
            {
                std::string channel;

                // Search for the channel of the current client
                for(uint32 i = 0; i < clients.size(); ++i)
                {
                    Client &client = clients[i];

                    if (client.ip == ip && client.port == port)
                    {
                        channel = client.channel;
                        break;
                    }
                }

                if (!channel.empty())
                {
                    std::cout << "Receive audio of channel : " << channel << std::endl;

                    std::string data = decapsulate(&receivedMessage);

                    std::string message = "AUDIO";
                    message += " ";
                    message += sint32ToString(clock.time());
                    message += " ";
                    message += data;

                    std::cout << "Data length : " << data.length() << std::endl;

                    // Send to all other clients connected to the channel
                    for(uint32 i = 0; i < clients.size(); ++i)
                    {
                        Client &client = clients[i];

                        if (client.channel == channel && client.ip != ip && client.port != port)
                        {
                            std::cout << "Send audio to client (" << client.ip << ":" << client.port << ")" << std::endl;
                            server.send(message, client.ip, client.port);
                        }
                    }
                }
            }

            if (header == "PING")
            {
                std::string message = "PONG";

                // Search for an existing client
                for(uint32 i = 0; i < clients.size(); ++i)
                {
                    Client &client = clients[i];

                    if (client.ip == ip && client.port == port)
                    {
                        // Update the time of a last message
                        client.timeLastMessage = clock.time();
                        break;
                    }
                }

                message += " ";
                message += sint32ToString(clock.time());

                server.send(message, ip, port);
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
