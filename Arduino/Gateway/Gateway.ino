#include "SoftwareSerial.h"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

SoftwareSerial serial(D6, D5); // rx, tx

void setup() {
  serial.begin(9600);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);

  // clear the string:
  inputString = "";
  stringComplete = false;
  serial.write("GetData");
}

void loop() {
  while (serial.available()) {
    // get the new byte:
    char inChar = (char)serial.read();
    if (inChar == 's') {
      stringComplete = true;
    }
    else{
       // add it to the inputString:
    inputString += inChar;
    } 
  }
  if (stringComplete == true ) {
    if (inputString == "on") {
      digitalWrite(D4, LOW);
      serial.println("turn on led");
    }
    else if (inputString == "off") {
      digitalWrite(D4, HIGH);
      serial.println("turn off led");
    }
    serial.println(inputString);
    inputString = "";
    stringComplete= false;
  }

  delay(200);
}
