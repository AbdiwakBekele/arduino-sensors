#include <LiquidCrystal.h>
int LEDpin = 8;
int percentage = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
  lcd.begin(16,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  int percentage = analogRead(0);

percentage = map(percentage, 600, 1023, 100, 0);
 Serial.println(percentage);
  lcd.clear();  // Clear the LCD screen
  lcd.setCursor(0, 0);  // Set cursor to the first column, first row
  lcd.print("Moisture: ");
  lcd.print(percentage);
  lcd.print("%");
 delay(100);

  if (percentage <= 50) {
    digitalWrite(LEDpin, HIGH);// turn water pump on
    delay(500);
  } else {
    digitalWrite(LEDpin, LOW); // turn water pump off
    Serial.println(percentage);
    delay(500);
  }
}