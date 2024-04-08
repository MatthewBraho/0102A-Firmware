

#include <LiquidCrystal.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d5 = 3, d7 = 2;


void setup() {
  lcd.begin(16, 2);
  lcd.print("Hello World");
}

void loop() {
  
  lcd.print("Arduino");
  delay(3000);

  lcd.setCursor(2, 1);
  lcd.print("fortnite");

  lcd.clear();

  
}
