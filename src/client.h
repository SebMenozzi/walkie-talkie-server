#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "types.h"

typedef struct
{
    std::string channel;
    std::string ip;
    uint16 port;
    sint32 timeLastMessage;

} Client;

#endif // CLIENT_H_INCLUDED
