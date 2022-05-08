#include "SoftwareSerial.h"
#include <string.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

SoftwareSerial serial(D6, D5); // rx, tx

void setup() {
  Serial.begin(9600);
  serial.begin(9600);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);

  // clear the string:
  inputString = "";
  stringComplete = false;
  Serial.println("Serial already");

  char str[]= "hello##da";
  char *token = NULL;
  token = strcat(str, "##");
}

void loop() {
  while (serial.available()) {
//     get the new byte:
    char inChar = (char)serial.read();
    Serial.println(inChar);
    if (inChar == 's') {
      stringComplete = true;
    }
    else {
//       add it to the inputString:
      inputString += inChar;
    }
  }
  if (stringComplete == true ) {
    if (inputString == "on") {
      digitalWrite(D4, LOW);
      serial.write("ens");
    }
    else if (inputString == "off") {
      digitalWrite(D4, HIGH);
      serial.write("dis");
    }
    Serial.println(inputString);
    inputString = "";
    stringComplete = false;
  }
  delay(20000);
}
