#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>
#include "types.h"

std::string decapsulate(std::string *message);
sint32 stringToSint32(std::string string);
float16 stringToFloat16(std::string string);
std::string sint32ToString(sint32 value);
std::string uint32ToString(uint32 value);

#endif // UTILS_H_INCLUDED
