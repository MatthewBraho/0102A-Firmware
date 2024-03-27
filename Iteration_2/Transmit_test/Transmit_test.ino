/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
const int moistureSensorPin1 = A0; // Define the pin for the first moisture sensor
const int ledPin1 = 2; // Define the pin for the first LED

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(ledPin1, OUTPUT); // Set the first LED pin as an output
}

void loop() {
  int moistureValue1 = analogRead(moistureSensorPin1); // Read the value from the first moisture sensor

  if (moistureValue1 < 300) {
    const char text[] = "DRY";
    //digitalWrite(ledPin1, HIGH); // Turn on the first LED if moisture value is below 300
    radio.write(&text, sizeof(text));
  } else {
    const char text[] = "WET";
    //digitalWrite(ledPin1, LOW); // Turn off the first LED if moisture value is 300 or above
    radio.write(&text, sizeof(text));
  }
  Serial.println("Sent");
  delay(1000);
}
