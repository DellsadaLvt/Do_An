#include <ESP8266WiFi.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = "MyASUS";   // your network SSID (name)
char pass[] = "00011101";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Weather station channel details
unsigned long myChannelNumber = 1513929;
const char * myWriteAPIKey = "RDGA8L7G5R6NJIQC";

// Initialize our values
int number1 = 0;
int number2 = random(0, 100);
String myStatus = "";

int statusCode = 0;
int field[8] = {1, 2};

void setup() {
  Serial.begin(9600);  // Initialize serial
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  Wifi_Connection();
  ThingSpeak_WriteData();
  ThingSpeak_ReadData();
  delay(20000); // Wait 20 seconds to update the channel again
}

void Wifi_Connection( void ) {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(2000);
    }
    Serial.println("\nConnected.");
  }
}

void ThingSpeak_WriteData( void ) {
  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);

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

    int windDir = ThingSpeak.getFieldAsInt(field[0]); // Field 1
    float windSpeed = ThingSpeak.getFieldAsFloat(field[1]); // Field 2

    String statusMessage = ThingSpeak.getStatus(); // Status message
    String createdAt = ThingSpeak.getCreatedAt(); // Created-at timestamp
    
    Serial.println("Status Message, if any: " + statusMessage);
    Serial.println("Wind Direction (North = 0 degrees): " + String(windDir));
    Serial.println("Wind Speed (mph): " + String(windSpeed));
    Serial.println("Created at, if any (YYYY-MM-DD hh:mm:ss): " + createdAt);
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }

  Serial.println();
}
