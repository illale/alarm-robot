const int echoPin = 13;
const int trigPin = 12;
const int tiltPin = 11;
const int ledPin = 10;
const int audPin = 3;

int tiltState = 0;
int previousTiltState = 0;
  
void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(tiltPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(audPin, OUTPUT);
}

void loop() {
  long distance;
  distance = getDistance();
  Serial.print(distance);
  Serial.print(" cm \n");
  tiltState = digitalRead(tiltPin);
  if (tiltState != previousTiltState) {
    digitalWrite(ledPin, LOW);  
  } else {
    digitalWrite(ledPin, HIGH);
    tone(audPin, 1000);
  }
}

long getDistance() {
  long duration, distance;
  // Cleares the trigger signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Tells the sensor to trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Reads the value coming form echo signal
  duration = pulseIn(echoPin, HIGH);

  // Turns the distance into cm's and returns it
  distance = duration*0.034/2;
  return distance;
}
