/*
 * Receiver Central Node Firmware for Request Protocol
 * 
 * Requests moisture data from sensors sequentially, and displays the data when request is fulfilled.
 * If the request receives no reply, then the probe it asked for requires maintence.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

//Pipe addresses
const uint64_t sensor_receieve_1 = 0xF0F0F0F0AA;
const uint64_t sensor_receieve_2 = 0xF0F0F0F066;
const uint64_t sensor_request = 0xF0F0F0F0F0;

//Array of pipe/sensor labels that receiever will call for
uint16_t request[] = {1,2};
const int numProbes = 2;

//How long to wait for a reply
unsigned long wait_time = 5000; 
//Placeholder to check which probe is answering its request
uint8_t payload_pipe = 0;

//Datapacket structure
struct data_packet {
  uint16_t moistureValue;
  uint16_t sensorNum;
};
//Initialise
data_packet transmitted_data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,sensor_receieve_1); //Sensor 1
  radio.openReadingPipe(2,sensor_receieve_2); //Sensor 2
  radio.openWritingPipe(sensor_request); //Pipe for requests
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
}

void loop() {
  delay(2000); //Time in between updates from probes

  //Call for the probes one by one
  for(int i = 0; i < numProbes; i++){
    payload_pipe = 0;
    radio.flush_rx(); //Flush the RX buffer so old values dont get read

    //Sending a request
    radio.stopListening(); 
    radio.write(&request[i], sizeof(request)); //Send a request to ith probe
    Serial.print("Request to sensor "); Serial.print(request[i]); Serial.println(" sent.");

    //Listening for a response
    radio.startListening();
    int start = millis(); //Start a timer to begin waiting for a response
    //Continue checking if bytes have arrived from the correct probe, if they arent arriving then break when wait_time ms has elapsed
    while((millis() - start) < wait_time && payload_pipe != request[i]) {
      radio.available(&payload_pipe);
    }
    
    //If the loop was broken due to exceeding the wait time, then give up on the call.
    if((millis()-start) >= wait_time){
      Serial.println("No response was heard.");
      delay(1000);
      //Jump to next probe.
      continue;      
    }

    //If a packet has arrived from the correct probe, read the data to transmitted_data
    radio.read(&transmitted_data, sizeof(transmitted_data));
    //Another check to see if it has received data from the correct probe. Display results if so.
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
