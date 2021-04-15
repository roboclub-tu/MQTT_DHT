#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Sensor
#define DHTPIN 4   
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

//DHT
unsigned long previousMillisDHT = 0;
const long intervalDHT = 2000;

//Wifi 
const char* ssid = "valkov";
const char* password = "08884584";

//MQTT
#define mqtt_server "192.168.0.100"
WiFiClient espClient; 
PubSubClient client(espClient); 
#define mqttTemp1 "home/temp1"
#define mqttHum1 "home/hum1"


void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  //tuk ppc trqbva da ima delay(500)   
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883); 
  client.connect("Client");
  }

void loop() {
  unsigned long currentMillisDHT = millis();
  if (currentMillisDHT - previousMillisDHT >= intervalDHT){
  previousMillisDHT = currentMillisDHT;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
   if (isnan(h) || isnan(t)) {
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
}
