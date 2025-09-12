#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

int count = 0;

void setup() {
    Serial.println("Setup iniziato.");
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX);
    radio.stopListening();
    Serial.begin(9600);
    Serial.println("Setup concluso!");
}

void loop() {
    if (radio.write(&count, sizeof(count))) {
        Serial.print("Inviato count: ");
        Serial.println(count);
        count++;
    }
}