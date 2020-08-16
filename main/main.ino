#include <Servo.h>
#include <WiFi.h>


/*
IDLE_STATE: Tarkistaa onko kello jo sen verran, milloin hälytys soi, ja kone alkaa ajamaan.
ALARM_STATE: Kone hälyttää ja ajaa eteenpäin.
OBSTACLE_STATE: Kone kääntyy väistäen esteen ja jatkaa hälyttämistä.

Tilanmuunnokset:
IDLE_STATE -> ALARM_STATE, kun aika on asetetun ajan verran.
ALARM_STATE -> OBSTACLE_STATE, kun huomataan este suoraan edessä.
ALARM_STATE -> IDLE_STATE, kun konetta heilutetaan, ja hälytys lopetetaan.
OBSTACLE_STATE -> ALARM_STATE, kun este on väistetty.
OBSTACLE_STATE -> IDLE_STATE, kun konetta heilutetaan, ja hälytyts lopetetaan.
*/
enum STATE { IDLE_STATE, ALARM_STATE, OBSTACLE_STATE, SETTINGS_STATE};
//Asetetaan ensimmäiseksi tilaksi IDLE_STATE
enum STATE MACHINE_STATE;

//Pinnit, joita ultraäänisensori käyttää
const int echoPin = 13;
const int trigPin = 12;

const int tiltPin = 11;
const int ledPin = 10;
const int audPin = 3;

//Pinnit pyörien moottoreille
const int rightPinActivate = 4;
const int rightPin5 = 5;
const int rightPin6 = 6;

const int leftPinActivate = 7;
const int leftPin8 = 8;
const int leftPin9 = 9;

//Servon nollapositio
int pos = 90;
Servo servo;

//Lippu, jonka avulla tarkistetaan onko moottori käynnistetty
bool IS_DRIVING = false;
bool CONTINUOS = false;
//Oletusarvo ajalle, jonka kuluttua hälytetään millisekuntteina
long wanted_time_interval = 200000;
long time = 5000;
long current_time;


void get_settings() {
    if (Serial.available() > 0) {
        int bytes[4];
        int byte = 0;
        int i = 0;
        int j = 0;
        long end_values[10];
        while((byte = Serial.read()) != -1) {
            //Tulevat arvot ovat 32 bittisiä, joten 4 tavua on yksi arvo
            bytes[i] = byte;
            if (i == 3) {
                //Olettaa että arvojen MSB on vasemmassa laidassa, eli listan ensimmäisen arvon ensimmäinen bitti on MSB
                end_values[j] = bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
                j++;
            } else if (i == 4) {
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
        }
      else {
        time = 5000;  
      }
}

void check_time() {
    //Tässä funktiossa tarkistetaan aika, ja jos aika == haluttu aika, niin vaihdetaan tilaan ALARM_STATE
    current_time = millis();
    if (time - current_time <= 0 && time != 0) {
        MACHINE_STATE = ALARM_STATE;
        //Tarkistetaan onko haluttu hälytys ns jatkuva vai pelkästään yhden kerran hälyttävä
        CONTINUOS ? time = 0 : time += wanted_time_interval;
    }
}

//Alustetaan moottori
void initMotor(int activationPin, int pin1, int pin2)
{
    pinMode(activationPin, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    digitalWrite(activationPin, HIGH); 
}

//Pyöritetään määrättyä moottoria eteen tai taakse
void drive(int pin1, int pin2, bool forward = true){
    if (forward) {
        //Ajetaan eteenpäin
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else {
        //Peruutetaan
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }
}

//Pysäyttää liikkeen
void stop(){
    digitalWrite(rightPin5, LOW);
    digitalWrite(rightPin6, LOW);
    digitalWrite(leftPin8, LOW);
    digitalWrite(leftPin9, LOW);
}

void move_machine() {
    //Tässä funktiossa "käynnistetään" moottori ja ajetaan eteenpäin.
    if (!IS_DRIVING) {
        //Start the motor here
        drive(leftPin8, leftPin9, true);
        drive(rightPin5, rightPin6, true);
    }
}

long getDistance() {
    //Lasketaan etäisyys
    long duration, distance;
   
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Hakee pulssin keston.
    duration = pulseIn(echoPin, HIGH);

    // Laskee etäisyyden kuluneen ajan perusteella. 
    distance = duration * 0.034 / 2;

    return distance;
}

void check_obstacles() {
    //Tässä funktiossa tarkistetaan onko koneen edessä esteitä, jos on siirytään tilaan OBSTACLE_STATE.
    int upper_bound, lower_bound;
    upper_bound = 10;
    lower_bound = 0;
    long value = getDistance();
    if (lower_bound <= value && value <= upper_bound) {
        MACHINE_STATE = OBSTACLE_STATE;
    }
}

void init_servo() {
    //Asetetaan servo alkuasentoon
    servo.write(pos);
}

void dodge_obstacles() {
    //Väistetään este kääntämällä "tornia" oikealle ja vasemmalle, jonka jälkeen päätetään kumpaan suuntaan käännytään.
    //Kääntyminen tapahtuu käyttämällä vain jompaa kumpaa moottoria oikeaa tai vasenta.
    servo.write(140);
    delay(1000);
    long distLeft = getDistance();
    servo.write(40);
    delay(1000);
    long distRight = getDistance();

    if (distLeft <= distRight){
        //Käännytään oikealle jos vasemmalla este
        drive(rightPin5, rightPin6, true);
        delay(2000);
        stop();
        
    } else {
        //Vasemmalle jos toisin päin
        drive(leftPin8, leftPin9, true);
        delay(2000);
    }
    servo.write(pos);
}

void detect_off_signal() {
    //Tarkistetaan heilutetaanko konetta
}

void setup() {
    MACHINE_STATE = IDLE_STATE;
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
            //detect_off_signal();
        case OBSTACLE_STATE:
            dodge_obstacles();
            //detect_off_signal();
            MACHINE_STATE = ALARM_STATE;
        case SETTINGS_STATE:
            get_settings();
            MACHINE_STATE = IDLE_STATE;
    }
}           
