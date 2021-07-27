#            Small home automation system using MQTT 
The main idea of this project is to create a basic home automation system using ESP32 as the main microcontroler.

The components used are: 
- EPS32
- Raspberry Pi 3
- HC-SR04 Ultrasonic Sensor
- DHT11 Humidity Sensor 
- Lamp
- Relay 


#           How does it work?  
MQTT is a lightweight, publish-subscribe network protocol that transports messages between devices. This is made possible using the Raspberry as a broker. this means that the information form the sensors are send to a "topic".
Then any device that subscribes to this "topic" can read all the information that is there.
![image](https://user-images.githubusercontent.com/49244252/127191130-c1143a79-678e-4f5a-ae5d-fd4082798b15.png)

Here using Node-RED after subscribing to the topic. the information from the sensors is visualised. 

#           How does the code for MQTT work? 

First we need to create the MQTT server. This is made using this code: 

```
IPAddress mqtt_server (192, 168, 0, 101);    

WiFiClient espClient; 

PubSubClient client(espClient); 
```
In the next snipper we create a Callback Function in which it reads what kind of message is read from the topic.
```
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
```
Subscribing and publishing is made like this: 
```
client.subscribe("home/top"); 
client.publish(mqttTemp1, String(t).c_str());
```
