#include <Servo.h>
#include <WiFi.h>

enum STATE = { IDLE_STATE=1, ALARM_STATE, OBSTACLE_STATE};
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

//Asetetaan ensimmäiseksi tilaksi IDLE_STATE
enum STATE MACHINE_STATE;
MACHINE_STATE = IDLE_STATE;

//Lippu, jonka avulla tarkistetaan onko moottori käynnistetty
bool IS_DRIVING = false;

//Pinnit, joita ultraäänisensori käyttää
const int echoPin = 13;
const int trigPin = 12;

const int tiltPin = 11;
const int ledPin = 10;
const int audPin = 3;

void get_settings() {

}

void check_time() {
    //Tässä funktiossa tarkistetaan aika, ja jos aika == haluttu aika, niin vaihdetaan tilaan ALARM_STATE
    if (time == wanted_time) {
        MACHINE_STATE = ALARM_STATE;
    } else {

    }

}

void move_machine() {
    //Tässä funktiossa "käynnistetään" moottori ja ajetaan eteenpäin.
    if (!IS_DRIVING) {
        //Start the motor here
    }
}

long check_distance() {
    //Laske etaisyys eteenpäin.
    return distance;
}

long getDistance() {
    //Laskee etaisyyden eteen
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
    distance = duration*0.034/2;

    return distance;
}

void check_obstacles() {
    //Tässä funktiossa tarkistetaan onko koneen edessä esteitä, jos on siirytään tilaan OBSTACLE_STATE.
    long value = getDistance();
    if (alaraja <= value && value <= ylaraja) {
        MACHINE_STATE = OBSTACLE_STATE;
    }
}

void dodge_obstacles() {
    //Väistetään este kääntämällä "tornia" oikealle ja vasemmale, jonka jälkeen päätetään kumpaan suuntaan käännytään.
    //Kääntyminen tapahtuu käyttämällä vain jompaa kumpaa moottoria oikeaa tai vasenta.
}

void detect_off_signal() {
    //Tarkistetaan heilutetaanko konetta
}

void setup() {
    Serial.begin(9600);
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(tiltPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(audPin, OUTPUT);
}

void loop() {
    switch MACHINE_STATE:
        case IDLE_STATE:
            check_time();
        case ALARM_STATE:
            move_machine();
            check_obstacles();
            detect_off_signal();
        case OBSTACLE_STATE:
            dodge_obstacle();
            detect_off_signal();
            MACHINE_STATE = ALARM_STATE;
}           
