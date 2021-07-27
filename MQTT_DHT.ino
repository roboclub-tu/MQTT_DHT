#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h> 

//Ultrasonic 
int distance; 
long duration;

#define Trig 33
#define Echo 32

//Sensor
#define DHTPIN 4   
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

//DHT
unsigned long previousMillisDHT = 0;
const long intervalDHT = 2000;

//Led 
int Led = 2; 
int Relay = 15; 

//Wifi 
const char* ssid = "valkov";
const char* password = "08884584";

//MQTT
IPAddress mqtt_server (192, 168, 0, 101);

WiFiClient espClient; 
PubSubClient client(espClient); 

#define mqttTemp1 "home/temp1"
#define mqttHum1 "home/hum1"


void setup()
{
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT); 
    
  pinMode(Led, OUTPUT); 
  pinMode(Relay, OUTPUT); 
  digitalWrite(Led, HIGH);
 
  Serial.begin(115200);
  dht.begin();
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

  client.subscribe("home/top"); 
}

void reconnect() {
  int counter = 0;
  while (!client.connected()) {
    if (counter==5){
      ESP.restart();
    }
    counter+=1;
    Serial.print("Attempting MQTT connection...");
   
   
    if (client.connect("client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      
    }
  }
}


void mqttCallback(char* topic, byte* message, unsigned int length)
{
  String messageL; 
  for ( int i=0; i < length; i++) { 
    messageL += (char)message[i];
  }
  if (String(topic) == "home/top") { 
    if(messageL == "on") { 
      digitalWrite(Relay, HIGH); 
    }
    else if(messageL == "off"){ 
      digitalWrite(Relay, LOW);
    }
  }
}

void ultrasonic()
{ 
  digitalWrite(Led, HIGH);
  digitalWrite(Trig, LOW);
  delayMicroseconds(2); 
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Echo, LOW);
  duration = pulseIn(Echo, HIGH); 
  
  distance = duration*0.034/2;
  
  if(distance < 10)
  {
    digitalWrite(Relay, HIGH); 
  }
  else 
  {
    digitalWrite(Relay, LOW); 
  }
  
}
  

void loop()
{
    if (!client.connected()){
    reconnect();
  }
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  unsigned long currentMillisDHT = millis();
  
  if (currentMillisDHT - previousMillisDHT >= intervalDHT)
{
  previousMillisDHT = currentMillisDHT;
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("sorry bratme"));
    return;
  }

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("C "));

    
  client.publish(mqttTemp1, String(t).c_str());
  client.publish(mqttHum1, String(h).c_str());
}
ultrasonic();
client.loop();
}
