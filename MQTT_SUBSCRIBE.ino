#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>

int led = 2; 
int relay = 15; 

//Wifi 
const char* ssid = "valkov";
const char* password = "08884584";

//MQTT
IPAddress mqtt_server (192, 168, 0, 101);

WiFiClient espClient; 
PubSubClient client(espClient); 


#define mqttLight "home/light"

void setup()
{

  pinMode(relay, OUTPUT); 
  pinMode(led, OUTPUT);
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  client.setCallback(mqttCallback);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883); 
  client.connect("Client");

  client.publish("home/top", "you already know who it is");
  client.subscribe("home/top");
}

void TopicSubscribe()
{ 
  client.subscribe("home/light");
}

void reconnect() 
{
  int counter = 0;
  while (!client.connected()) {
    if (counter==5){
      ESP.restart();
    }
    counter+=1;
    Serial.print("Attempting MQTT connection...");
   
   
    if (client.connect("client")) {
      Serial.println("connected");
      TopicSubscribe();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      
    }
  }
  TopicSubscribe();
}

void mqttCallback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("MQTT message received on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageL;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageL += (char)message[i];
  }
  Serial.println();
  Serial.println(messageL);
}
 
void loop()
{
   if (!client.connected()){
    reconnect();
  }

  client.loop();
}
