#include <ESP8266WiFi.h>

//const int ON = 1;    // Constant to indicate that lights are on
//const int OFF = 2;  // Constant to indicate that lights are off
const char* ssid="Your network SSID";
const char* pass="Your password";
const String APIKEY = "Replace with your Carriots apikey"; 
const String DEVICE = "Replace with the id_developer of your device"; 


IPAddress server(82,223,244,60);  // api.carriots.com IP Address

WiFiClient client; // Initialize the library instance

int ledPin = 13;        // Led pin number
int LDRPin = A0;        // LDR sensor pin number

int lights = LOW;       // Current status
int newLights = LOW;    // New status


void setup() {
 
delay(1000);
  pinMode(ledPin, OUTPUT);        // Initialize the digital pin as an output

  Serial.begin(115200);             // Start serial port
  Serial.println("initializing");
 //ESP.wdtFeed();
  WiFi.begin(ssid,pass);         // Start the Ethernet connection
  
  while(WiFi.status()!=WL_CONNECTED)
  {
  //  delay(100);
    Serial.print(".");
    delay(200);
    }
   
  Serial.println("WIFI CONNECTED");               // Give the Ethernet shield a second to initialize

}

// The loop routine runs over and over again forever
void loop() {
 
  int val = analogRead(LDRPin);   // Read the value from the sensor
  Serial.println(val);

  if (val > 900) {  // This is the value limit between day or night with our LDR sensor. Maybe you need adjust this value.
    newLights = LOW;             // Now its night. We have to turn on the LED
    digitalWrite(ledPin, HIGH);   // Turn the LED on (HIGH is the voltage level)
  }
  else {
    newLights = HIGH;               // Now its day. We have to turn off the LED
    digitalWrite(ledPin, LOW);    // Turn the LED off by making the voltage LOW
  }

  if (lights != newLights) {        // Check if we have a change in status
    Serial.println("Send Stream");
    lights = newLights;             // Status update and send stream
    sendStream();
  }

  delay(500);

  // If theres incoming data from the net connection, send it out the serial port
  // This is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
      client.stop();
  }
}

// Send stream to Carriots
void sendStream()
{
  String txt = "";          // Text to send
  if ( lights == LOW ) {   // Alarm OFF
     txt = "OFF";
  } else {                  // Alarm ON
     txt = "ON";
  }
  Serial.println(txt);      // For debugging purpose only

  if (client.connect(server, 80)) {   // If theres a successful connection
    Serial.println("Client connected");
    // Build the data field
    String json = "{\"protocol\":\"v2\",\"device\":\""+DEVICE+"\",\"at\":1356390000,\"data\":{\"light\":\""+txt+"\"}}";

    // Make a HTTP request
    client.println("POST /streams HTTP/1.1");
    client.println("Host: api.carriots.com");
    client.println("Accept: application/json");
    client.println("User-Agent: Arduino-Carriots");
    client.println("Content-Type: application/json");
    client.print("carriots.apikey: ");
    client.println(APIKEY);
    client.print("Content-Length: ");
    int thisLength = json.length();
    client.println(thisLength);
    client.println("Connection: close");
    client.println();

    client.println(json);
  }
  else {
    // If you didnt get a connection to the server:
    Serial.println("connection failed");
  }
}
   
