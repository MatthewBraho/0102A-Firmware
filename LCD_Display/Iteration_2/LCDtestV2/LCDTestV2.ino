#include <LiquidCrystal.h>
#include <stdlib.h>

//This code creates simulates recieving a new moisture value every 3 seconds
//and then updating the LCD display accordingly
//This is just to create a proxy of the soil measurement communication, and allows for testing with only the lcd on the breadboard
// this will also proxy the press of a button
const int rs = 2 , en = 3, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  Serial.begin(31250);
}

int button_num = 0;
int moist = 300;
int prevMoist = 0;

void loop() {

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(prevMoist);
  // Display the previous moisture values

  lcd.setCursor(0, 1);
  lcd.print(moist);
  lcd.setCursor(8, 1);

  if ( moist > 300 & moist != prevMoist) {
  // determine whether the current moisture value is unique, and is above a certain threshold
    lcd.print("MOIST");

  } else {
    lcd.print("DRY");
  }

  prevMoist = moist;
  moist = rand() % 1000 + 1;
  //come up with a new moisture value
  delay(2000); 

  
}