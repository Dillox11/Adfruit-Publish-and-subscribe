
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "********" 
#define WIFI_PASS "*************"

#define ADAFRUIT_IO_USER "*********" 
#define ADAFRUIT_IO_KEY  "************"      

const int LDR_PIN = 35; 
const int ledPin=27;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, ADAFRUIT_IO_USER, ADAFRUIT_IO_KEY);
Adafruit_MQTT_Publish lightFeed = Adafruit_MQTT_Publish(&mqtt, ADAFRUIT_IO_USER "/feeds/ldr");
Adafruit_MQTT_Subscribe ledFeed = Adafruit_MQTT_Subscribe(&mqtt, ADAFRUIT_IO_USER "/feeds/led");
Adafruit_MQTT_Publish smsFeed = Adafruit_MQTT_Publish(&mqtt, ADAFRUIT_IO_USER "/feeds/sms");

void connectToAIO() {
  int retries = 3;
  while (mqtt.connected() == false) {
    Serial.print("Connecting to Adafruit IO... ");
    mqtt.connect();
    retries--;
    if (retries == 0) {
      ESP.restart();
    }
    delay(500);
  }
  Serial.println("Connected to Adafruit IO!");
}

void handleMessage(char *data, uint16_t len) {
  Serial.print("Received new data: ");
  Serial.println(data);

  // Check the value of the received data
  if (strcmp(data, "1") == 0) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED is now ON");
    function1();
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED is now OFF");
    function2();
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("LDR Sensor Publisher");

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  
  pinMode(LDR_PIN, INPUT);

  ledFeed.setCallback(handleMessage);
  mqtt.subscribe(&ledFeed);
}

void loop() {
  // This function listens for new messages and calls the callback
  mqtt.processPackets(100);
  
  if (!mqtt.connected()) {
    connectToAIO();
  }
  
  int ldrValue = analogRead(LDR_PIN);
  int Values=map(ldrValue,0,3500,0,100);

  // Publish the LDR value to the Adafruit IO feed
  if (lightFeed.publish((float)Values)) {
    Serial.print("Published LDR value: ");
    Serial.println(Values);
  } else {
    Serial.println("Failed to publish.");
  }
  delay(3000);
}
void function1()
{
  if (smsFeed.publish("lamp is ON")) 
  {
    
  } 
}
void function2()
{
  if (smsFeed.publish("lamp is OFF")) 
  {
    
  } 
}
