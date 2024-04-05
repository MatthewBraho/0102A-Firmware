/*
 * Receiver Central Node Firmware for Request Protocol
 * 
 * Requests moisture data from sensors sequentially, and displays the data when request is fulfilled.
 * If the request receives no reply, then the probe it asked for requires maintence.
*/

//This basic integration simply displays and updates the values of one of transmitter values
#include <LiquidCrystal.h>
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
unsigned long buffer_time = 250;
//Placeholder to check which probe is answering its request
uint8_t payload_pipe = 0;

//Datapacket structure
struct data_packet {
  uint16_t moistureValue;
  uint16_t sensorNum;
};
//Initialise
data_packet transmitted_data;

const int rs = 2, en = 4, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int buttonPin = 3;
int button_num = 0;

const int LED_1 = 9;
const int LED_2 = 5;
int LED_all[] = {LED_1, LED_2};
const int moist_threshold = 300;
long counter = 0;
long max_count = 400001;

void buttonInterrupt () { //Interrupt function that listens for the rising of the button
  if(button_num == (numProbes -1)) {
    button_num = 0;
  } else {
    button_num += 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor " + String(button_num + 1));
  lcd.setCursor(0, 1);
  lcd.print("Pending");

  while (counter < max_count){
  counter++;                 // will increment counter by 1 
  if (counter == 100000)     // Check if counter value reach to 100000
  {
    digitalWrite(LED_all[button_num], HIGH); // turn LED ON by writing HIGH (Sending 5v to pin 13)
  }
  else if (counter == 200000) // Check if counter value reach to 200000
  {
    digitalWrite(LED_all[button_num], LOW);  // turn LED OFF by writing LOW (Sending 0v to pin 13)
  }
  //Code is non blocking you can add your loop code after this
  else if (counter == 300000) // Check if counter value reach to 200000
  {
    digitalWrite(LED_all[button_num], HIGH);  // turn LED OFF by writing LOW (Sending 0v to pin 13)
  }
  else if (counter == 400000) // Check if counter value reach to 200000
  {
    digitalWrite(LED_all[button_num], LOW);  // turn LED OFF by writing LOW (Sending 0v to pin 13)
  }
  }
  counter = 0;
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,sensor_receieve_1); //Sensor 1
  radio.openReadingPipe(2,sensor_receieve_2); //Sensor 2
  radio.openWritingPipe(sensor_request); //Pipe for requests
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, RISING);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
}

void loop() {
  delay(2000); //Time in between updates from probes

  //Call for the probes one by one
  for(int i = 0; i < numProbes; i++){
    delay(2000);
    payload_pipe = 0;
    radio.flush_rx(); //Flush the RX buffer so old values dont get read

    //Sending a request
    radio.stopListening(); 

    unsigned long start = millis(); //Start a timer to begin waiting for a response
    while(millis()-start < buffer_time){
      radio.write(&request[i], sizeof(request)); //Send a request to ith probe
    }
    Serial.print("Request to sensor "); Serial.print(request[i]); Serial.println(" sent.");

    //Listening for a response
    radio.startListening();
    
    start = millis(); //Start a timer to begin waiting for a response
    //Continue checking if bytes have arrived from the correct probe, if they arent arriving then break when wait_time ms has elapsed
    while((millis() - start) < wait_time && payload_pipe != request[i]) {
      //radio.flush_rx();
      radio.available(&payload_pipe);
    }
    
    //If the loop was broken due to exceeding the wait time, then give up on the call.
    if((millis()-start) >= wait_time){
      // Serial.println(payload_pipe);
      Serial.println("No response was heard.");
      if(button_num == i) {
        lcd.clear(); 
        lcd.print("Problem with");
        lcd.setCursor(0, 1);
        lcd.print(String("Sensor ") + String(button_num+1));
        delay(1000);
      }
      //Jump to next probe.
      continue;      
    }

    //If a packet has arrived from the correct probe, read the data to transmitted_data
    radio.read(&transmitted_data, sizeof(transmitted_data));
    //Another check to see if it has received data from the correct probe. Display results if so.
    if(transmitted_data.sensorNum == request[i]){
      Serial.print("Request to sensor "); Serial.print(request[i]); Serial.println(" was successful.");\
      Serial.print("Sensor: "); Serial.print(transmitted_data.sensorNum); Serial.print(" | Moisture Value: "); Serial.println(transmitted_data.moistureValue);
      //This allows for printing to the LCD display if there is no value recieved from the LCD screen
      if(transmitted_data.moistureValue < moist_threshold){
        digitalWrite(LED_all[transmitted_data.sensorNum], HIGH); // If moisture level is below the threshold, turn the light on
      }
      else{
        digitalWrite(LED_all[transmitted_data.sensorNum], LOW); // If the moisture level is above the threshold, turn off the light
      }

      if(button_num == i) { 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String("Sensor ") + String(button_num+1) + String(":"));
        lcd.setCursor(0, 1);
        lcd.print(transmitted_data.moistureValue);
      } 
      
      delay(1000);
    }
    else{
      Serial.print("Request to sensor "); Serial.print(request[i]); Serial.println(" was unsuccessful.");
      delay(1000);
    }
  }
  
}

