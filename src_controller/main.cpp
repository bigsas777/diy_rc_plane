#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <shared.h>

#define ENGINE_STICK_PIN A3
#define X_ALIERONS_STICK_PIN A1
#define Y_ALIERONS_STICK_PIN A0
#define CE_PIN 7
#define CSN_PIN 8

#define MIN_RIGHT_SERVO_DEGREES 180 // 180 - 0
#define MID_RIGHT_SERVO_DEGREES 150 // 180 - 30
#define MAX_RIGHT_SERVO_DEGREES 110 // 180 - 70
#define MIN_LEFT_SERVO_DEGREES 0
#define MID_LEFT_SERVO_DEGREES 30 // Same as right, added for consistency and readability
#define MAX_LEFT_SERVO_DEGREES 70
#define MIN_SPEED 1000         // Pulse of 1ms for min RPM on brsuhless motor
#define MAX_SPEED 2000         // Pulse of 2ms for max RPM
#define VERTICAL_THRESHOLD 100
#define HORIZONTAL_THRESHOLD 5


RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void print_payload(packet payload);

void setup() {
    Serial.begin(9600);

    // Setting up wireless module
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.stopListening();
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
            p.right_alieron_pos = MAX_RIGHT_SERVO_DEGREES;
            p.left_alieron_pos = MAX_LEFT_SERVO_DEGREES;
        } else { // Spin zone (flying straight up)
            p.right_alieron_pos = MIN_RIGHT_SERVO_DEGREES;
            p.left_alieron_pos = MIN_LEFT_SERVO_DEGREES;
        }
    } else if (turning < -HORIZONTAL_THRESHOLD || turning > HORIZONTAL_THRESHOLD) { // Turning zone (no pitch zone)
        if (turning < 0) { // Turning right
            short abs_turning = abs(turning);
            p.right_alieron_pos = map(abs_turning, 0, 512, MID_RIGHT_SERVO_DEGREES, MAX_RIGHT_SERVO_DEGREES);
            p.left_alieron_pos = MID_LEFT_SERVO_DEGREES - map(abs_turning, 0, 512, MIN_LEFT_SERVO_DEGREES, MID_LEFT_SERVO_DEGREES);
        } else { // Turning left
            p.right_alieron_pos = MID_RIGHT_SERVO_DEGREES - map(turning, 0, 512, MIN_RIGHT_SERVO_DEGREES, MID_RIGHT_SERVO_DEGREES);
            p.left_alieron_pos = map(turning, 0, 512, MID_LEFT_SERVO_DEGREES, MAX_LEFT_SERVO_DEGREES);
        }
    } else {
        p.right_alieron_pos = MID_RIGHT_SERVO_DEGREES;
        p.left_alieron_pos = MID_LEFT_SERVO_DEGREES;
    }

    // Payload preparation
    radio.write(&p, sizeof(p));
    // print_payload(p);
}