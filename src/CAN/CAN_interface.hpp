#pragma once
#include <ctime>
#include <cstdint>

typedef struct can_message_s
{
    time_t timestamp;
    uint16_t id;
    uint8_t dlc;
    uint8_t data[8];

    can_message_s() : timestamp(0), id(0), dlc(0), data{0}
    {
    }
} can_message_t;