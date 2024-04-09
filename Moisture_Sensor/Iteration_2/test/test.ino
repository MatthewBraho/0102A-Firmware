/* 
# Script to read in data from sensor and output number

# Connect the sensor1 to the A0(Analog 0) pin on the Arduino board
# Connect sensor2 to the A1 pin

# Connect the led1 to output 13
# Connect the led2 to output 12
*/ 

const int moistureSensorPin1 = A0; // Define the pin for the first moisture sensor
const int ledPin1 = 12; // Define the pin for the first LED


void setup() {
  Serial.begin(57600);
  pinMode(ledPin1, OUTPUT); // Set the first LED pin as an output

}

void loop() {
  int moistureValue1 = analogRead(moistureSensorPin1); // Read the value from the first moisture sensor

  Serial.print("Moisture Sensor 1 Value: ");
  Serial.println(moistureValue1);

  if (moistureValue1 < 511.5) {
    digitalWrite(ledPin1, HIGH); // Turn on the first LED if moisture value is below 300
    Serial.print("DRY");

  } else {
    digitalWrite(ledPin1, LOW); // Turn off the first LED if moisture value is 300 or above
  }


  delay(100);
}
