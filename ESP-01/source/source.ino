#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "********";
const char* password = "********";

const char* mqttServer = "public.mqtthq.com";
const int mqttPort = 1883;
const char* mqttUser = "user";
const char* mqttPassword = "password";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("Message arrived in topic: ");
    Serial.print(topic);
  
    Serial.println("Message: ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
}

void setup() {
  
  //Serial.begin(115200);
  Serial.begin(9600);
  
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected"); 
    } else {
      Serial.println("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe("esp/test");
  client.publish("esp/test", "I'm connected");

  
  
}

void loop() {
  client.loop();

  Serial.println("Write something");
  while(Serial.available() <= 0);
  
  String incomingText = Serial.readString();
  
  Serial.println("Read: ");
  Serial.print(incomingText);
  
  client.publish("esp/test", incomingText.c_str());
}
