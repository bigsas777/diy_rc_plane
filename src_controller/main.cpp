#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define ENGINE_STICK_PIN A3
#define X_ALIERONS_STICK_PIN A1
#define Y_ALIERONS_STICK_PIN A0
#define CE_PIN 7
#define CSN_PIN 8

#define MIN_SERVOS_DEGREES 0   // Useless, created only for consistency
#define MID_SERVOS_DEGREES 90  // Useless, created only for consistency
#define MAX_SERVOS_DEGREES 180
#define MIN_SPEED 1000         // Pulse of 1ms for min RPM on brsuhless motor
#define MAX_SPEED 2000         // Pulse of 2ms for max RPM
#define VERTICAL_THRESHOLD 100
#define HORIZONTAL_THRESHOLD 5

typedef struct {
    short engine_speed;
    short right_alieron_pos;
    short left_alieron_pos;
} packet;

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void print_payload(packet payload);

void setup() {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX);
    radio.stopListening();
    Serial.begin(9600);
}

void loop() {
    packet p;
    short pitch, turning;

    // Reading joysticks
    short tmp_engine = map(analogRead(ENGINE_STICK_PIN), 0, 1023, 1023, 0);
    p.engine_speed = map(max(500, tmp_engine), 500, 1023, MIN_SPEED, MAX_SPEED);

    pitch = analogRead(X_ALIERONS_STICK_PIN);
    turning = analogRead(Y_ALIERONS_STICK_PIN) - 512;

    // Alierons values interpretation
    if (pitch < 512 - VERTICAL_THRESHOLD || pitch > 512 + VERTICAL_THRESHOLD) { // Pitch zone has the priority
        if (pitch < 512) { // Nosedive zone
            p.right_alieron_pos = MAX_SERVOS_DEGREES;
            p.left_alieron_pos = MAX_SERVOS_DEGREES;
        } else { // Spin zone (flying straight up)
            p.right_alieron_pos = MIN_SERVOS_DEGREES;
            p.left_alieron_pos = MIN_SERVOS_DEGREES;
        }
    } else if (turning < -HORIZONTAL_THRESHOLD || turning > HORIZONTAL_THRESHOLD) { // Turning zone (no pitch zone)
        if (turning < 0) { // Turning right
            short abs_turning = abs(turning);
            p.right_alieron_pos = map(abs_turning, 0, 512, MID_SERVOS_DEGREES, MAX_SERVOS_DEGREES);
            p.left_alieron_pos = MID_SERVOS_DEGREES - map(abs_turning, 0, 512, MIN_SERVOS_DEGREES, MID_SERVOS_DEGREES);
        } else { // Turning left
            p.right_alieron_pos = MID_SERVOS_DEGREES - map(turning, 0, 512, MIN_SERVOS_DEGREES, MID_SERVOS_DEGREES);
            p.left_alieron_pos = map(turning, 0, 512, MID_SERVOS_DEGREES, MAX_SERVOS_DEGREES);
        }
    } else {
        p.right_alieron_pos = MID_SERVOS_DEGREES;
        p.left_alieron_pos = MID_SERVOS_DEGREES;
    }

    // Payload preparation
    radio.write(&p, sizeof(p));
    print_payload(p);
}

// For debug use only
void print_payload(packet payload) {
    Serial.print("Engine: ");
    Serial.println(payload.engine_speed);
    Serial.print("Left: ");
    Serial.println(payload.left_alieron_pos);
    Serial.print("Right: ");
    Serial.println(payload.right_alieron_pos);
    Serial.print("");
    delay(100);
}