//TRANSMITTER FILE
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 8); // CE, CSN

String option;
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  while(Serial.available()==0){}
  
  String command = Serial.readStringUntil('\r');
  
  int N = command.length();
  char text[N] = {};
  

  for(int i=0;i<N;i++){
   text[i]=command[i];
  }

  
  radio.write(&text, sizeof(text));
  delay(10);
}
