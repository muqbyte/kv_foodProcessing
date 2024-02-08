#include <WiFi.h>
#include <MQTT.h>
#include <SoftwareSerial.h>
#include "max6675.h"

int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

MQTTClient client;
const char clientId[] = "haha"; //MQTT ID
const char server[]   = "txio.uitm.edu.my"; //Broker
const char publishmqtt1[] = "KV/18/temp"; //Publish

WiFiClient net;
const char ssid[]     = "Moon Knight_2.4GHz";
const char password[] = "satusampailapan";

void setup() {
  // initialize serial communication
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.begin(server, 1883, net);
  
  Serial.println("MAX6675 test");
  delay(3000);  // wait for the MQ2 to warm up
}

void loop() {
  //check MQTT connection
  checkConnect();
  DataProcessing();
}

void checkConnect(){
  client.loop();

  if (!client.connected()) {
    connect();
  }
}

void connect() {
  
  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

  Serial.print("Connecting to MQTT broker ...");
  while (!client.connect(clientId)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

 // client.subscribe(topic);
 client.subscribe(publishmqtt1);
}

void DataProcessing(){
  if (runEvery(3000)) {
  float tempCelsius=thermocouple.readCelsius();
  float tempFahrenheit=thermocouple.readFahrenheit();
  Serial.println(tempCelsius);
  client.publish(publishmqtt1, String(tempCelsius));
  }
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
