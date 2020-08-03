#include <Servo.h>

//Oikea moottori
const int rightPinActivate = 4;
const int rightPin5 = 5;
const int rightPin6 = 6;

//Vasen moottori
const int leftPinActivate = 7;
const int leftPin8 = 8;
const int leftPin9 = 9;

//Servon nollapositio
int pos = 90;
Servo servo;

//Alustetaan moottori
void initMotor(int activationPin, int pin1, int pin2)
{
    pinMode(activationPin, OUTPUT);
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    digitalWrite(activationPin, HIGH); 
}

void init_servo() {
    //Asetetaan servo alkuasentoon
    servo.write(pos);
}

//Moottorin ajaminen eteen tai taakse
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

//Pysäyttää pyörät
void stop(){
    digitalWrite(rightPin5, LOW);
    digitalWrite(rightPin6, LOW);
    digitalWrite(leftPin8, LOW);
    digitalWrite(leftPin9, LOW);
}

//Servo kakkospinniin ja moottorien määritys
void setup(){ 
    initMotor(leftPinActivate, leftPin8, leftPin9);
    initMotor(rightPinActivate, rightPin5, rightPin6);
    servo.attach(2);
}

void dodge_obstacles(){
    //Väistetään este kääntämällä tornia oikealle ja vasemmalle, jonka jälkeen päätetään kumpaan suuntaan käännytään.
    //Kääntyminen tapahtuu käyttämällä vain jompaa kumpaa moottoria oikeaa tai vasenta.
    servo.write(140);
    delay(1000);
    long distLeft = getDistance();
    servo.write(40);
    delay(1000);
    getDistance();
    long distRight = getDistance();

    if (distLeft <= distRight){
        //Käännytään oikealle jos vasemmalla jotain lähellä
        drive(rightPin5, rightPin6, true);
        delay(2000);
        stop();
    } else{
        //Vasemmalle jos toisin päin
        drive(leftPin8, leftPin9, true);
        delay(2000);
    }
    servo.write(pos);
}

//Demonstraatio toiminnasta TinkerCadille
void loop(){
    //Ajetaan eteen ja peruutetaan saman verran
    drive(leftPin8, leftPin9, true);
    drive(rightPin5, rightPin6, true);
    delay(1000);

    drive(leftPin8, leftPin9, false);
    drive(rightPin5, rightPin6, false);
    delay(1000);

    stop();
    delay(1000);

    //Kääntyminen onnistuu pyörittämällä vain toista moottoria
    //Vasemmalle
    drive(rightPin5, rightPin6, true);
    delay(2000);
    stop();

    //Oikealle
    drive(leftPin8, leftPin9, true);
    delay(2000);

    stop();
    dodge_obstacles();
}
