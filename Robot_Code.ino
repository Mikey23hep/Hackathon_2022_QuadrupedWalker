/*PROJECT INFORMATION=========================================================
  HACKATHON ROBOT
  11-11-2022
  Michael Heppner, Mohammed Rashed, Jacob Lowe
  ============================================================================*/
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//TRANSMITTER/RECIEVER-----------------------------------
//Initializes Communication to a Transmitter
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

//ULTRA SONIC SENSOR-------------------------------------
// defines pins numbers
const int trigPin = 3;
const int echoPin = 2;

long duration;
int distance;
bool safe = true;

//SERVO MOTORS-------------------------------------------
//INITIALIZE PIN VALUES
const int FR_ROT_PIN = 27;
const int FL_ROT_PIN = 26;
const int BR_ROT_PIN = 25;
const int BL_ROT_PIN = 24;

const int FR_UD_PIN = 33;
const int FL_UD_PIN = 32;
const int BR_UD_PIN = 31;
const int BL_UD_PIN = 30;

//INITIALIZE SERVOS
//Rotating Joint Servos
Servo FR_ROT_LEG;
Servo FL_ROT_LEG;
Servo BR_ROT_LEG;
Servo BL_ROT_LEG;

//Up Down (UD legs)
Servo FR_UD_LEG;
Servo FL_UD_LEG;
Servo BR_UD_LEG;
Servo BL_UD_LEG;

//System Variables
String state;
String command;

//Function Prototypes
void STRUT_FORWARD();
void STRUT_BACKWARDS();
void STRUT_LEFT();
void STRUT_RIGHT();
bool COLLISION_SYSTEM();

//======================================================================================
//Setup Routine Function
//======================================================================================
void setup() {

  Serial.begin(9600);

  //Initializes communication with Transmitter and Begin Recieving Data
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //DO NOT KNOW IF WE WILL USE THIS AT ALL
  //Distance starts at 0
  double distance = 0;

  //Attach pins to servos rotating vertically.
  FR_ROT_LEG.attach(FR_ROT_PIN);
  FL_ROT_LEG.attach(FL_ROT_PIN);
  BR_ROT_LEG.attach(BR_ROT_PIN);
  BL_ROT_LEG.attach(BL_ROT_PIN);

  //Attach pins to servos rotating horizontally
  FR_UD_LEG.attach(FR_UD_PIN);
  FL_UD_LEG.attach(FL_UD_PIN);
  BR_UD_LEG.attach(BR_UD_PIN);
  BL_UD_LEG.attach(BL_UD_PIN);

  //SET SERVO POSITIONS on initialization
  FR_UD_LEG.write(45);
  FL_UD_LEG.write(135);
  BR_UD_LEG.write(135);
  BL_UD_LEG.write(45);
  delay(200);

  //SET SERVO POSITIONS on initialization
  FR_ROT_LEG.write(90);
  FL_ROT_LEG.write(90);
  BR_ROT_LEG.write(90);
  BL_ROT_LEG.write(90);
  delay(200);

  //Set Servo Position on intitialization
  FR_UD_LEG.write(90);
  FL_UD_LEG.write(90);
  BR_UD_LEG.write(90);
  BL_UD_LEG.write(90);
  delay(200);

  //ULTRASONIC SENSOR----------------------------------------------------
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  state = "Manual";

  //delay 5 seconds before hitting main routine
  delay(5000);
}

//======================================================================================
//Main Routine Loop Function
//======================================================================================
void loop() {
  command="";
  //Recieves A character array from the transmitter
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    command = String(text);
  }

  //Current state----------------------------------------
  if (command == "AutoRun") {
    state = "Auto";
  }
  //Else If Not Auto
  else if (command == "AutoOff") {
    state = "Manual";
  }

  //Serial.println(state);

  //Action off specified state
  if (state == "Auto") {
    if (COLLISION_SYSTEM() == true) {
      STRUT_FORWARD();
    }
    else if (command == "Reset") {
      setup();
    }
    else {
      STRUT_LEFT();
    }
  }
  else {
    if (command == "Forward") {
      STRUT_FORWARD();
    }
    else if (command == "Backward") {
      STRUT_BACKWARD();
    }
    else if (command == "Left") {
      STRUT_LEFT();
    }
    else if (command == "Right") {
      STRUT_RIGHT();
    }
    else if (command == "Reset") {
      setup();
    }
  }


  delay(2000);
}

//======================================================================================
//USER MADE Functions
//======================================================================================

//Motion of the Motor--------------------------------------------
//MOVE THE BOT FORWARD
void STRUT_FORWARD() {
  for (int i = 0; i < 6; i++) {
    //Move BL LEG
    BL_UD_LEG.write(60);
    delay(100);
    BL_ROT_LEG.write(30);
    delay(100);
    BL_UD_LEG.write(90);
    delay(100);

    BR_UD_LEG.write(120);
    delay(100);
    BR_ROT_LEG.write(150);
    delay(100);
    BR_UD_LEG.write(90);
    delay(100);

    FR_ROT_LEG.write(30);
    BL_ROT_LEG.write(90);
    FL_ROT_LEG.write(150);
    BR_ROT_LEG.write(90);
    delay(200);

    FR_UD_LEG.write(60);
    delay(100);
    FR_ROT_LEG.write(90);
    delay(100);
    FR_UD_LEG.write(90);
    delay(100);

    FL_UD_LEG.write(120);
    delay(100);
    FL_ROT_LEG.write(90);
    delay(100);
    FL_UD_LEG.write(90);
    delay(100);
  }
}

//MOVE THE BOT BACKWARD
void STRUT_BACKWARD() {
  for (int i = 0; i < 5; i++) {
    FR_UD_LEG.write(60);
    delay(100);
    FR_ROT_LEG.write(30);
    delay(100);
    FR_UD_LEG.write(90);
    delay(100);

    FL_UD_LEG.write(120);
    delay(100);
    FL_ROT_LEG.write(150);
    delay(100);
    FL_UD_LEG.write(90);
    delay(100);

    FR_ROT_LEG.write(90);
    BL_ROT_LEG.write(30);
    FL_ROT_LEG.write(90);
    BR_ROT_LEG.write(150);
    delay(200);

    //Move BL LEG
    BL_UD_LEG.write(60);
    delay(100);
    BL_ROT_LEG.write(90);
    delay(100);
    BL_UD_LEG.write(90);
    delay(100);

    BR_UD_LEG.write(120);
    delay(100);
    BR_ROT_LEG.write(90);
    delay(100);
    BR_UD_LEG.write(90);
    delay(100);
  }
}
//MOVE THE BOT RIGHT
void STRUT_RIGHT() {
  for (int i = 0; i < 6; i++) {
    //Move FR LEG
    FR_UD_LEG.write(60);
    delay(100);
    FR_ROT_LEG.write(30);
    delay(100);
    FR_UD_LEG.write(90);
    delay(100);

    BL_UD_LEG.write(60);
    delay(100);
    BL_ROT_LEG.write(30);
    delay(100);
    BL_UD_LEG.write(90);
    delay(100);

    FL_UD_LEG.write(120);
    delay(100);
    FL_ROT_LEG.write(30);
    delay(100);
    FL_UD_LEG.write(90);
    delay(100);

    BR_UD_LEG.write(120);
    delay(100);
    BR_ROT_LEG.write(30);
    delay(100);
    BR_UD_LEG.write(90);
    delay(100);

    FR_ROT_LEG.write(90);
    BL_ROT_LEG.write(90);
    FL_ROT_LEG.write(90);
    BR_ROT_LEG.write(90);
    delay(200);
  }
}

//MOVE THE BOT LEFT
void STRUT_LEFT() {
  for (int i = 0; i < 5; i++) {
    //Move FR LEG
    FR_UD_LEG.write(60);
    delay(100);
    FR_ROT_LEG.write(150);
    delay(100);
    FR_UD_LEG.write(90);
    delay(100);

    BL_UD_LEG.write(60);
    delay(100);
    BL_ROT_LEG.write(150);
    delay(100);
    BL_UD_LEG.write(90);
    delay(100);

    FL_UD_LEG.write(120);
    delay(100);
    FL_ROT_LEG.write(150);
    delay(100);
    FL_UD_LEG.write(90);
    delay(100);

    BR_UD_LEG.write(120);
    delay(100);
    BR_ROT_LEG.write(150);
    delay(100);
    BR_UD_LEG.write(90);
    delay(100);

    FR_ROT_LEG.write(90);
    BL_ROT_LEG.write(90);
    FL_ROT_LEG.write(90);
    BR_ROT_LEG.write(90);
    delay(200);
  }
}

//COLLISION SYSTEM
bool COLLISION_SYSTEM() {

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034;
  Serial.println(distance);
  if (distance < 15) {
    return false;
  }
  else {
    return true;
  }
}
