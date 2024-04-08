 /*
* Transmitter Node for Request Protocol
* 
* Probe will be checking for requests continously. When it receives a packet calling its name, send a packet back with moisture sensor data.
* If the packet is addressing another probe, then do not send any data back.
* 
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

//Pipeline addresses for transmitting and receiving
const uint64_t transmit_pipe = 0xF0F0F0F066;
const uint64_t request_pipe = 0xF0F0F0F0F0;
//Moisture analog pin
const int moistureSensorPin = A0;

//Storing the labeling variable
uint16_t request = 0;
//Buffer to allow for signal to be read
unsigned long buffer_time = 500;

struct data_packet{
  uint16_t moistureValue;
  uint16_t sensorNum;
};

data_packet sensor2_packet;

void setup() {
  radio.begin();
  radio.openWritingPipe(transmit_pipe);
  radio.openReadingPipe(1, request_pipe);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  sensor2_packet.sensorNum = 2;
}

void loop() {
  radio.startListening();

  //If it receives a request, read who its addressed for and assign it to request
  if(radio.available()){
    radio.read(&request, sizeof(request));
    //If the request is addressed to this probe (2), send a data packet back.
    Serial.println(request);
    if(request == 2){
      radio.stopListening();
      unsigned long start = millis();
      
      //Continously read sensor data and send it off for 500 ms to allow for reciever to pick up on it.
      while(millis()-start < buffer_time){
        int moistureValue = analogRead(moistureSensorPin);
        sensor2_packet.moistureValue = moistureValue;
        radio.write(&sensor2_packet, sizeof(sensor2_packet));
      }
      Serial.println("Request Heard, Data sent.");
    }
    else{
      Serial.println("Not talking to me!");
    }

  }
}
