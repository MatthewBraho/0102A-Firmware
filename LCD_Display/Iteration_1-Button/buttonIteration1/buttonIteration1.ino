#include <LiquidCrystal.h>
#include <stdlib.h>

//This code creates simulates recieving a new moisture value every 3 seconds
//and then updating the LCD display accordingly
//This is just to create a proxy of the soil measurement communication, and allows for testing with only the lcd on the breadboard

const int rs = 2, en = 3, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int button_num = 0;
int buttonStatus = 0;
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(4, INPUT);
}



void loop() {
  if(digitalRead(4)) {
    delay(50);
    if(digitalRead(4)) {
      button_num+=1;
      lcd.clear();
      lcd.print(button_num);
    }  
  } 
  Serial.println(digitalRead(4));
  delay(100);
  
  
}