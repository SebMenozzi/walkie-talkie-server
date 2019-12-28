#include "clock.h"
#include <time.h>

Clock::Clock()
{
    timeBeginning = this->getTicks();
}

void Clock::adjust(sint32 time)
{
    this->timeBeginning = this->getTicks() - time;
}

sint32 Clock::time(void)
{
    return this->getTicks() - this->timeBeginning;
}

sint32 Clock::getTicks(void)
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    return (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000);
}
