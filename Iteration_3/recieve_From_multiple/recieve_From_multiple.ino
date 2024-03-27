/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
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
const uint64_t sensor2 = 0xF0F0F0F066;

struct data_packet {
  int moistureValue;
  int sensorNum;
};
data_packet transmitted_data;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,sensor1);
  radio.openReadingPipe(2,sensor2);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  transmitted_data.sensorNum = 5;
}

void loop() {

  if (radio.available()) {
    radio.read(&transmitted_data, sizeof(transmitted_data));
    Serial.println(transmitted_data.sensorNum);
    //Serial.print("Sensor: "); Serial.print(transmitted_data.sensorNum); Serial.print(" | Moisture Value: "); Serial.println(transmitted_data.moistureValue);

    //radio.read(&transmitted_data, sizeof(transmitted_data));
    //Serial.print("Sensor: "); Serial.print(transmitted_data.sensorNum); Serial.print(" | Moisture Value: "); Serial.println(transmitted_data.moistureValue);
  }
}
