int LEDpin = 13;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDpin, OUTPUT); 
  digitalWrite(LEDpin, HIGH);  
}

void loop() {
  // repeatedly turning LED on and off ever 2s
  digitalWrite(LEDpin, HIGH);
  delay(2000); // wait 2s
  digitalWrite(LEDpin, LOW);
  delay(2000);
}
