#include <ESP8266WiFi.h>
#include <string.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include "SoftwareSerial.h"

/* section for handle uart data */
#define NOTHING (0U)
#define TEM_MSG (1U)
#define HUM_MSG (2U)

/* Wifi inf */
char ssid[] = "MyASUS";   // your network SSID (name)
char pass[] = "00011101";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// ThingSpeak Inf
unsigned long myChannelNumber = 1513929;
const char * myWriteAPIKey = "RDGA8L7G5R6NJIQC";

// Data convert from uart data receive from stm to send to thingSpeak
int number1 = 0;
int number2 = random(0, 100);
String myStatus = "";

/* Section ThingSpeak receive */
int statusCode = 0;
int field[8] = {1, 2};
uint8_t EspCmd= 0u;

/* Section for uart */
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
SoftwareSerial serial(D6, D5); // rx, tx

/* Section for divide tasks */
unsigned long long ll_g_CurrentTimeTask_0 = 0u;
unsigned long long ll_g_CurrentTimeTask_1 = 0u;

void setup() {
  Serial.begin(9600);  // Initialize serial
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  serial.begin(9600);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
  // clear the string:
  inputString = "";
  stringComplete = false;
  
  Serial.println("Serial already");

  ll_g_CurrentTimeTask_0 = millis();
  ll_g_CurrentTimeTask_1 = millis();
}

void loop() {
  
  Wifi_Connection();
  if ( millis() - ll_g_CurrentTimeTask_0 >= 20000u ){
    ll_g_CurrentTimeTask_0 = millis();  
    ThingSpeak_WriteData();
    ThingSpeak_ReadData();
  }

  if ( millis() - ll_g_CurrentTimeTask_1 >= 1000u ){
    Serial.println("Com task");
    ll_g_CurrentTimeTask_1 = millis();
    Com_ReceiveData();
    Com_SendData(EspCmd);
  }
}

/*  */
uint8_t StringToInt ( String str, uint8_t len ){
  uint8_t u8l_ret = 0u;

  if ( len == 1u ){
    u8l_ret = ( str[0u] - 48u );
  }
  else if( len == 2u ){
    u8l_ret = ( ( str[0] - 48u ) * 10u ) + ( str[1u] - 48u );
  }
  else if( len == 3u ){
    u8l_ret = ( ( str[0] - 48u ) * 100u ) + ( ( str[1] - 48u ) * 10u ) + ( str[2u] - 48u );
  }
  else if( len == 4u ){
    u8l_ret = ( ( str[0] - 48u ) * 1000u ) + ( ( str[1] - 48u ) * 100u ) + ( ( str[2] - 48u ) * 10u ) + ( str[3u] - 48u );
  }
 
  return u8l_ret;
}

void Com_ReceiveData( void ){
  uint8_t u8l_MsgType = NOTHING;
  uint8_t u8a_CountChar = 0u;
  while (serial.available()) {
//     get the new byte:
    char inChar = (char)serial.read();
    u8a_CountChar += 1u;
    
    if ( inChar == 't' ) {
      stringComplete = true;
      u8l_MsgType = TEM_MSG;
    }
    else if ( inChar == 'h' ) {
      stringComplete = true;
      u8l_MsgType = HUM_MSG;
    }
    else {
      // add it to the inputString:
      inputString += inChar;
    }
  }
  /* Send data */
  if (stringComplete == true ) {
    if( u8l_MsgType == TEM_MSG ){
      number1 = StringToInt(inputString, u8a_CountChar - 1u);
      Serial.print("count char: ");
      Serial.println(u8a_CountChar);
      u8a_CountChar = 0u;
      
      Serial.println("Send TEMPERATURE");
      Serial.print("value TEMP in int: ");
      Serial.println(number1);
    }

    else if( u8l_MsgType == HUM_MSG ){
      number2 = StringToInt(inputString, u8a_CountChar - 1u);
      Serial.print("count char: ");
      Serial.println(u8a_CountChar);
      u8a_CountChar = 0u;
      
      Serial.println("Send HUMIDITY");
      Serial.print("value HUMI in int: ");
      Serial.println(number2);
    }
//    Serial.print("Count char: ");
//    Serial.println(u8a_CountChar);
    Serial.print("Value in String type: ");
    Serial.println(inputString);
//    u8a_CountChar = 0u;
    Serial.println("-------------");
    
    inputString = "";
    stringComplete = false;
  }
}

void Com_SendData( uint8_t data ){
  serial.write(data);
}

void Wifi_Connection( void ) {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}

void ThingSpeak_WriteData( void ) {
  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  Serial.println("the int value ");
  Serial.println(number1);
  Serial.println(number2);

  // set the status
  ThingSpeak.setStatus(myStatus);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // change the values
  number1++;
  if (number1 > 99) {
    number1 = 0;
  }
  number2 = random(0, 100);
}

void ThingSpeak_ReadData( void ) {
  // Read and store all the latest field values, location coordinates, status message, and created-at timestamp
  // use ThingSpeak.readMultipleFields(channelNumber, readAPIKey) for private channels
  statusCode = ThingSpeak.readMultipleFields(myChannelNumber, "PYOHF0Z3CKGXDNVR");

  if (statusCode == 200)
  {
    // Fetch the stored data

    EspCmd = ThingSpeak.getFieldAsInt(field[0]); // Field 1
    uint8_t Feild_2_value = ThingSpeak.getFieldAsFloat(field[1]); // Field 2

    String statusMessage = ThingSpeak.getStatus(); // Status message
    String createdAt = ThingSpeak.getCreatedAt(); // Created-at timestamp
    
    Serial.println("Status Message, if any: " + statusMessage);
    Serial.println("Esp cmd: " + String(EspCmd));
    Serial.println("Feild 2 value: " + String(Feild_2_value));
    Serial.println("Created at, if any (YYYY-MM-DD hh:mm:ss): " + createdAt);
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }

  Serial.println();
}
