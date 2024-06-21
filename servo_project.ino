#include <Servo.h>

Servo myservo;

int pos = 0;
int cm = 0;

long readUltrasonicDistance(int triggerPin, int echoPin) {

  pinMode(triggerPin, OUTPUT);       // sets the trigger pin as output
  digitalWrite(triggerPin, LOW);     // sets the trigger pin off
  delayMicroseconds(2);              // waits for 2 ms
  digitalWrite(triggerPin, HIGH);    // sets the trigger pin on
  delayMicroseconds(10);              // waits for 10 ms
  digitalWrite(triggerPin, LOW);     // sets the trigger pin off
  pinMode(echoPin, INPUT);           // sets the digital echoPin as input
  
  return pulseIn(echoPin, HIGH);
}

void setup() {
  digitalWrite(12,LOW); // sets pin 12 to low
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  cm =0.01723 * readUltrasonicDistance(6,7);
  if(cm < 30) {
    Serial.print(cm);
    Serial.println("cm");
    for (pos = 0; pos <= 120; pos += 1) {
      myservo.write(pos);
      delay(15);
    }
    delay(500);

    for(pos = 120; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(15);
    }
    delay(15);
  }
}