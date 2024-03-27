/*
 * Receiver Firmware for Request Protocol
 * 
 * Requests moisture data from sensors sequentially, and displays the data when request is fulfilled.
 * If the request receives no reply, then the probe it asked for requires maintence. If the 
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const uint64_t sensor_receieve_1 = 0xF0F0F0F0AA;
const uint64_t sensor_receieve_2 = 0xF0F0F0F066;
const uint64_t sensor_request = 0xF0F0F0F0F0;

uint16_t request[] = {1,2};
const int numProbes = 2;
unsigned long wait_time = 5000; 

uint8_t payload_pipe = 0;

struct data_packet {
  uint16_t moistureValue;
  uint16_t sensorNum;
};
data_packet transmitted_data;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,sensor_receieve_1);
  radio.openReadingPipe(2,sensor_receieve_2);
  radio.openWritingPipe(sensor_request);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
}

void loop() {
  delay(2000);
  for(int i = 0; i < numProbes; i++){
    payload_pipe = 0;
    radio.flush_rx();
    radio.stopListening();
    radio.write(&request[i], sizeof(request));
    Serial.print("Request to sensor "); Serial.print(request[i]); Serial.println(" sent.");
    
    radio.startListening();
    int start = millis();
    while((millis() - start) < wait_time && payload_pipe != request[i]) {
      radio.available(&payload_pipe);
    }
    if((millis()-start) >= wait_time){
      Serial.println("No response was heard.");
      delay(1000);
      continue;      
    }
    radio.read(&transmitted_data, sizeof(transmitted_data));
    
    if(transmitted_data.sensorNum == request[i]){
      Serial.print("Request to sensor "); Serial.print(request[i]); Serial.println(" was successful.");
      Serial.print("Sensor: "); Serial.print(transmitted_data.sensorNum); Serial.print(" | Moisture Value: "); Serial.println(transmitted_data.moistureValue);
      delay(1000);
    }
    else{
      Serial.print("Request to sensor "); Serial.print(request[i]); Serial.println(" was unsuccessful.");
      delay(1000);
    }
  }
  
}
