 /*
* Request system transmitter
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const uint64_t transmit_pipe = 0xF0F0F0F0AA;
const uint64_t request_pipe = 0xF0F0F0F0F0;
const int moistureSensorPin = A0; 

struct data_packet{
  uint16_t moistureValue;
  uint16_t sensorNum;
};

data_packet sensor1_packet;
uint16_t request = 0;
unsigned long buffer_time = 500;

void setup() {
  radio.begin();
  radio.openWritingPipe(transmit_pipe);
  radio.openReadingPipe(1, request_pipe);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  sensor1_packet.sensorNum = 1;
}

void loop() {
  radio.startListening();
  if(radio.available()){
    radio.read(&request, sizeof(request));
    if(request == 1){
      radio.stopListening();
      int start = millis();
      while(millis()-start < buffer_time){
        int moistureValue = analogRead(moistureSensorPin);
        sensor1_packet.moistureValue = moistureValue;
        radio.write(&sensor1_packet, sizeof(sensor1_packet));
      }
      Serial.println("Request Heard, Data sent.");
    }
    else{
      Serial.println("Not talking to me!");
    }

  }
  
  
  
}
