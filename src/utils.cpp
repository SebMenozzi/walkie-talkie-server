#include "utils.h"
#include <sstream>

std::string decapsulate(std::string* message)
{
    std::string header;

    // Find the position of the first space
    size_t endHeader = message->find(' ');

    // No space anymore
    if (std::string::npos == endHeader)
    {
        // The header is the message itself
        header = *message;

        // The messages doesn't encapsulate anything, it is empty
        message->clear();

        return header;
    }

    // Retrieve the header
    header = message->substr(0, endHeader);

    // Delete the header in the message
    *message = message->substr(endHeader + 1);

    return header;
}

sint32 stringToSint32(std::string string)
{
    std::stringstream stream;
    stream << string;
    sint32 value;
    stream >> value;

    return value;
}

float16 stringToFloat16(std::string string)
{
    std::stringstream stream;
    stream << string;
    float16 value;
    stream >> value;

    return value;
}

std::string sint32ToString(sint32 value)
{
    std::stringstream stream;
    stream << value;
    std::string string;
    stream >> string;

    return string;
}

std::string uint32ToString(uint32 value)
{
    std::stringstream stream;
    stream << value;
    std::string string;
    stream >> string;

    return string;
}
