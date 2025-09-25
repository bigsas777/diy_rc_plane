#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#include <shared.h>

#define CE_PIN 7
#define CSN_PIN 8
#define ESC_PIN 10
#define RIGHT_ALIERON_PIN 3
#define LEFT_ALIERON_PIN 5

#define MIN_SPEED 1000
#define MAX_SPEED 2000
#define MID_RIGHT_SERVO_DEGREES 150
#define MID_LEFT_SERVO_DEGREES 30


RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

Servo engine;
Servo right_alieron;
Servo left_alieron;

void print_payload(packet payload);

void setup() {
    Serial.begin(9600);

    // Setting up wireless module
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.startListening();

    // Alierons to idle position
    right_alieron.attach(RIGHT_ALIERON_PIN);
    left_alieron.attach(LEFT_ALIERON_PIN);
    right_alieron.write(MID_RIGHT_SERVO_DEGREES);
    left_alieron.write(MID_LEFT_SERVO_DEGREES);
    
    // Setting up ESC
    engine.attach(ESC_PIN);
    engine.writeMicroseconds(MAX_SPEED);
    delay(2000);
    engine.writeMicroseconds(MIN_SPEED);
    delay(3000); // Wait for armings
}

void loop() {
    if(radio.available()) {
        packet p;
        radio.read(&p, sizeof(packet));
        // print_payload(p);

        engine.writeMicroseconds(p.engine_speed);
        right_alieron.write(p.right_alieron_pos);
        left_alieron.write(p.left_alieron_pos);
    }
}