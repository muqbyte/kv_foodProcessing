#include <WiFi.h>
#include <MQTT.h>
#include <SoftwareSerial.h>
#define AO_PIN 34  // ESP32's pin GPIO36 connected to AO pin of the MQ2 sensor

MQTTClient client;
const char clientId[] = "KVESP111"; //MQTT ID
const char server[]   = "txio.uitm.edu.my"; //Broker
const char publishmqtt1[] = "KV/18/temp"; //Publish
const char subscribemqtt[] = "KV/18/esp32"; //Subscribe own

WiFiClient net;
const char ssid[]     = "SEA-IC";
const char password[] = "seaic2022";

void setup() {
  // initialize serial communication
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.begin(server, 1883, net);
  //connect_wifi_mqtt();
  Serial.println("Warming up the MQ2 sensor");
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
  int gasValue = analogRead(AO_PIN);

  Serial.print("MQ2 sensor AO value: ");
  Serial.println(gasValue);
  //Serial.println("hello");
  client.publish(publishmqtt1, String(gasValue));
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
