#ifndef SHARED_H
#define SHARED_H

#include <Arduino.h>

typedef struct {
    int engine_speed;
    int left_alieron_pos;
    int right_alieron_pos;
} packet;

// For debug use only
void print_payload(packet payload);

#endif
