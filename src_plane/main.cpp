#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#include <shared.h>

#define CE_PIN 7
#define CSN_PIN 8
#define ESC_PIN 0 //! TO SET

#define MIN_SPEED 1000 // Useless, added only for readability
#define MID_SERVOS_DEGREES 90 // Useless, added only for readability

typedef struct {
    short engine_speed;
    short right_alieron_pos;
    short left_alieron_pos;
} packet;

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

Servo engine;
Servo right_alieron;
Servo left_alieron;

void print_payload(packet payload);

void setup() {
    Serial.begin(9600);
    Serial.println("Setup effettuato!");

    // Setting up wireless module
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    // Alierons to idle position
    right_alieron.write(MID_SERVOS_DEGREES);
    left_alieron.write(MID_SERVOS_DEGREES);
    
    // Setting up ESC
    engine.attach(ESC_PIN);
    engine.writeMicroseconds(MIN_SPEED);
    delay(3000); // Wait for arming //! TO TEST
}

void loop() {
    if(radio.available()) {
        packet p;
        radio.read(&p, sizeof(packet));
        print_payload(p);

        engine.writeMicroseconds(p.engine_speed);
        right_alieron.write(p.right_alieron_pos);
        left_alieron.write(p.left_alieron_pos);
    }
}