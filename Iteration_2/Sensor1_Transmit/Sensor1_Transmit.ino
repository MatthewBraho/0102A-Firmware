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

const uint64_t sensor1 = 0xF0F0F0F0AA;
const int moistureSensorPin = A0; 

struct data_packet{
  int moistureValue;
  int sensorNum;
};

data_packet sensor1_packet;

void setup() {
  radio.begin();
  radio.openWritingPipe(sensor1);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  //sensor1_packet.sensorNum = 1;
}

void loop() {
  delay(2000);
  int moistureValue = analogRead(moistureSensorPin); // Read the value from the first moisture sensor
  sensor1_packet.moistureValue = moistureValue;
  sensor1_packet.sensorNum = 1;
  radio.write(&sensor1_packet, sizeof(sensor1_packet));
  Serial.println(" Sent");
}
