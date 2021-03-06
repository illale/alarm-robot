int readValue;
int count = 0;
int previous;

void setup()
{
  pinMode(11, INPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}

//setting up alarm sound and if robot has been shaken the alarm stops
void alarm(int i) {
  if (i < 6) {
    tone(10, 100, 200);
    delay(10);
    tone(10, 300, 400);
    delay(200);
    tone(10, 50, 300);
    delay(150);
  }
  else {
    //do nothing
  }
}

//counts how many times alarm-robot has been shaken
void shake() {
  alarm(count);
  previous = readValue;
  readValue = digitalRead(11);
  if (readValue != previous) {
    count++;
  }
}

void loop()
{
  shake();
}
