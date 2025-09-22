#include "shared.h"

void print_payload(packet payload) {
    Serial.println("Ricevuto:");
    Serial.print("Engine: ");
    Serial.println(payload.engine_speed);
    Serial.print("Left: ");
    Serial.println(payload.left_alieron_pos);
    Serial.print("Right: ");
    Serial.println(payload.right_alieron_pos);
    Serial.print("");
    delay(100);
}