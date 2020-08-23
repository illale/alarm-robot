#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);

void setup()
{
  lcd.begin(16,2);
  Serial.begin(9600);
}

unsigned long time = 100000000;

void loop()
{
  lcd.setCursor(2, 0);
  // print the number of seconds since reset:
  lcd.print(time / 3600000);
  time = time - millis();
  Serial.print("Time  is: ");
  Serial.print(time);
  Serial.print("\n");

}