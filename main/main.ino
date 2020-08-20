#include <Servo.h>
#include <WiFi.h>


/*
IDLE_STATE: In this state, the arduino checks is the current running time higher than the wanted
            interval
ALARM_STATE: The robot alarms, tries to escape from the human
OBSTACLE_STATE: The robot detects obstacle, evades it, and continues to sound the alarm
SETTINGS_STATE: Check the settings from serial

State changes:
IDLE_STATE -> ALARM_STATE, when the running time is higher than the interval
ALARM_STATE -> OBSTACLE_STATE, when the robot notices obstacle in its way 
ALARM_STATE -> IDLE_STATE, when the robot is shaken by human, this results in the alarm stopping
OBSTACLE_STATE -> ALARM_STATE, when the obstacle has been avoided
OBSTACLE_STATE -> IDLE_STATE, when the robot is shaken by human, this results in the alarm stopping
SETTINGS_STATE -> IDLE_STATE, when duration from latest reset is over 30 seconds
*/
enum STATE { IDLE_STATE, ALARM_STATE, OBSTACLE_STATE, SETTINGS_STATE};

//Set the first state to IDLE_STATE
enum STATE MACHINE_STATE;

//Pins that the ultrasound sensor uses
const int echoPin = 13;
const int trigPin = 12;

//Pin used for tilt sensor
const int tiltPin = 11;

//const int ledPin = 10;

//Pin for the audio device
const int audPin = 3;

//Pins for the motors
const int rightPinActivate = 4;
const int rightPin5 = 5;
const int rightPin6 = 6;

const int leftPinActivate = 7;
const int leftPin8 = 8;
const int leftPin9 = 9;

//Zero position of the servo
int pos = 90;
Servo servo;

//Flag which is used for detecting if the motors has been started
bool IS_DRIVING = false;

//Flag that is used for continuos alarm:
//If false, then there is no second alarm, and vice versa
bool CONTINUOS = false;
//Default value for time interval in milliseconds
long time;
long wanted_time_interval;
long current_time;
//setupping values for alarm and detecting off signal
int readValue;
int count = 0;
int previous;


void get_settings() {
    //Check if there is data in serial channel, and if there is no available data set time to default value
    if (Serial.available() > 0) {

        int bytes[3];
        int byte = 0;
        int i = 0;
        int j = 0;
        long end_values[10];
        while((byte = Serial.read()) != -1) {
            //Incoming values should be 32 bit values
            bytes[i] = byte;
            if (i == 1) {
                //Transform the first two 16 bit values into one 32 bit value
                //This should be the value for the time interval
                end_values[j] = bytes[0] << 16 | bytes[1] << 0;
                j++;
            } else if (i == 2) {
                //This is the final value for the continuos FLAG
                end_values[j] = bytes[i];
                break;
            } else {
                i++;
            }
        }
        time = end_values[0];
        if (end_values[1] == 1) {
            CONTINUOS = true;
        }
    } else {
        time = 50;  
      }
    //Store the wanted time inteval, so that time is never doubled
    wanted_time_interval = time;
}

void check_time() {
    //Check if the wanted time interval has been crossed, and change the state accodingly

    //Check the current running time
    current_time = millis();

    if (time - current_time <= 0 && time != 0) {
        MACHINE_STATE = ALARM_STATE;
        //Check if alarm is contiunos and add another interval accordingly
        CONTINUOS ? time = 0 : time += wanted_time_interval;
    }
}

//Initialise the motors
void initMotor(int activationPin, int pin1, int pin2)
{
    pinMode(activationPin, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    digitalWrite(activationPin, HIGH); 
}

//Spin the motors forward or backwards 
void drive(int pin1, int pin2, bool forward = true){
    if (forward) {
        //Drive forward
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else {
        //Reverse
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }
}

//Stop the motors
void stop(){
    digitalWrite(rightPin5, LOW);
    digitalWrite(rightPin6, LOW);
    digitalWrite(leftPin8, LOW);
    digitalWrite(leftPin9, LOW);
}

void move_machine() {
     //Turn the motors on if they havent been turned on
    if (!IS_DRIVING) {
        //Start the motor here
        drive(leftPin8, leftPin9, true);
        drive(rightPin5, rightPin6, true);
    }
}

long getDistance() {
    //Calculate the distance to object in front of the ultrasound sensor
    long duration, distance;
   
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    //Get the length of the pulse
    duration = pulseIn(echoPin, HIGH);

    //Calculate the distance based on the time the pulse took
    distance = duration * 0.034 / 2;

    return distance;
}

void check_obstacles() {
    //Check if there are any obstacles infront of the robot and change state accordingly
    int upper_bound, lower_bound;
    upper_bound = 10;
    lower_bound = 0;
    long value = getDistance();
    if (lower_bound <= value && value <= upper_bound) {
        MACHINE_STATE = OBSTACLE_STATE;
    }
}

void init_servo() {
    //Initialize the servo to the starting position
    servo.write(pos);
}

void dodge_obstacles() {
    //Select the "better" direction for the robot by turning the tower to the left and right
    //and calculating the distancecs of which the shorter is dismised
    //The robot is turned with using only left or right motor
    servo.write(140);
    delay(1000);
    long distLeft = getDistance();
    servo.write(40);
    delay(1000);
    long distRight = getDistance();

    if (distLeft <= distRight){
        //Turn to the right if obstacle at left
        drive(rightPin5, rightPin6, true);
        delay(2000);
        stop();
        
    } else {
        //And to left if vice versa
        drive(leftPin8, leftPin9, true);
        delay(2000);
    }
    servo.write(pos);
}

void alarm(int i) {
    if (i < 6) {
        tone(audPin, 100, 200);
        delay(10);
        tone(audPin, 300, 400);
        delay(200);
        tone(audPin, 50, 300);
        delay(150);
        }
    else {
        MACHINE_STATE = IDLE_STATE;
    }
}

void detect_off_signal() {
    alarm(count);
    previous = readValue;
    readValue = digitalRead(tiltPin);
    if (readValue != previous) {
        count++;
    }
}

void setup() {
    MACHINE_STATE = SETTINGS_STATE;
    Serial.begin(9600);
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(tiltPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(audPin, OUTPUT);
    initMotor(leftPinActivate, leftPin8, leftPin9);
    initMotor(rightPinActivate, rightPin5, rightPin6);
    servo.attach(2);
}

void loop() {
    switch (MACHINE_STATE) {
        case IDLE_STATE:
            check_time();
        case ALARM_STATE:
            move_machine();
            check_obstacles();
            detect_off_signal();
        case OBSTACLE_STATE:
            dodge_obstacles();
            detect_off_signal();
            MACHINE_STATE = ALARM_STATE;
        case SETTINGS_STATE:
            while ( millis() < 3000) {
                //Check the setting in the first 30 seconds
                get_settings();
            }
            MACHINE_STATE = IDLE_STATE;
    }
}           
