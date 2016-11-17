#include <ESP8266WiFi.h>
const char* ssid="HARLEEN KAUR";
const char* pass="KARMANSINGH2712"; 
const char* server="api.carriots.com";
const String APIKEY ="cfe5dbe96edb81af5b09cea1690182b4329987d7f3f13d0390825516b9096828";
const String DEVICE ="PIR@harleen.harleen";

int ledPin=13;
int pirPin=12;
int val=0;
int pirstate=LOW;

WiFiClient client;

void setup() {
Serial.begin(115200);
pinMode(ledPin,OUTPUT);
pinMode(pirPin,INPUT);
Serial.println("CALIBERATING");
for(int i=0;i<20;i++)
{
  Serial.println(".");
  delay(100);
  }
  Serial.println();
Serial.println("CONNECTING TO SSID");
WiFi.begin(ssid,pass);
if(WiFi.status()!=WL_CONNECTED)
{
  Serial.println(".");
  delay(500);
}
 Serial.println("WIFI CONNECTED"); 
 Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void sendStream()
{
  String txt="";
  if(pirstate==HIGH)
 { txt="MOTION DETECTED";
    } else {                  // Alarm ON
   txt = "Something wrong";
  }
  Serial.println(txt);
  if(client.connect(server,80))
  {
    Serial.println("CONNECTED TO SERVER");
    String json = "{\"protocol\":\"v2\",\"device\":\""+DEVICE+"\",\"at\":\"now\",\"data\":{\"light\":\""+txt+"\"}}";
 
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
    // If you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}


void loop() {
val=digitalRead(pirPin);
Serial.println(val);
if(val==1)
{
    digitalWrite(ledPin,LOW); 
  if(pirstate==LOW)
  {
    Serial.println("MOTION DEECTED");
    pirstate=HIGH;
    Serial.println("SENDING STREAM");
    sendStream();
    delay(5000);
    } 
  else
  {
    Serial.println("MOTION IS STILL THERE");
    pirstate=HIGH;
    digitalWrite(ledPin,LOW);
   delay(5000);
    }  
  }
else
{
  digitalWrite(ledPin,HIGH);
  if(pirstate==HIGH)
  {
     Serial.println("MOTION DEECTED EMAIL SENT");
      pirstate=LOW;
    
  }
}
}
