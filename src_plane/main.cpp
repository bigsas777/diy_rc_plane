#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8
#define ESC_PIN 0 // TO SET

typedef struct {
    short engine_speed;
    short right_alieron_pos;
    short left_alieron_pos;
} packet;

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// Servo ESC;
// ESC.attach(ESC_PIN);

void print_payload(packet payload);

void setup() {
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    Serial.begin(9600);
    Serial.println("Setup effettuato!");
}

void loop() {
    if(radio.available()) {
        packet p;
        radio.read(&p, sizeof(packet));
        print_payload(p);
    }
}

// In plane's code use ESC.writeMicroseconds() for motor control

// For debug use only
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