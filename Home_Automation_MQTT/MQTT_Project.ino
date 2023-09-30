/*
Publishing in the callback

- connects to an MQTT server

This example shows how to control pins using Ethernet shield via MQTT
controling multiple light and read temperature sensor and display it.

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#define ROOM1_LIGHT1 7
#define ROOM1_LIGHT2 6
#define ROOM1_LIGHT3 5
#define ROOM3_Temperature 4
#define ROOM2_Temperature 3
#define ROOM1_Temperature 2

#define DHTTYPE DHT11


DHT dht1(ROOM1_Temperature, DHTTYPE);
DHT dht2(ROOM2_Temperature, DHTTYPE);
DHT dht3(ROOM3_Temperature, DHTTYPE);

float t1,t2,t3;
float hic1,hic2,hic3;
float h1,h2,h3;

char temp1[8], temp2[8], temp3[8];


/* Update these with values suitable for your network.*/
byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);

/*broqer address*/
IPAddress server(127, 0, 0, 1);
//const char* mqtt_server = "test.mosquitto.org";

/* Callback function header*/
void callback(char* topic, byte* payload, unsigned int length);

/*create rthernet client instance*/
EthernetClient ethClient;
/*create pubsub client instance*/
PubSubClient client(server, 1883, callback, ethClient);


/* Callback function*/
void callback(char* topic, byte* payload, unsigned int length)
{

Serial.print("Message: ");
/*turn the LED ON if the payload is '1' and publish to the MQTT server a confirmation message*/
// For Room1 light
if(strcmp(topic,"homedemo/room1/light") == 0)
{
  if(payload[0] == '1')
  {
    digitalWrite(ROOM1_LIGHT1, HIGH);
    client.publish("home/feedback", "Room1 Light On");
    Serial.print("Message: Room1 Light On");
  }
  /*turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message*/
  if (payload[0] == '0')
  {
    digitalWrite(ROOM1_LIGHT1, LOW);
    client.publish("home/feedback", "Room1 Light Off");
  }  
} // For Room2 light
else if(strcmp(topic,"homedemo/room2/light") == 0)
{
  if(payload[0] == '1')
  {
    digitalWrite(ROOM1_LIGHT2, HIGH);
    client.publish("home/feedback", "Room2 Light On");
    Serial.print("Message: Room2 Light On");
  }
  /*turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message*/
  if (payload[0] == '0')
  {
    digitalWrite(ROOM1_LIGHT2, LOW);
    client.publish("home/feedback", "Room2 Light Off");
  }  
} // For Room3 light
else if(strcmp(topic,"homedemo/room3/light") == 0)
{
  if(payload[0] == '1')
  {
    digitalWrite(ROOM1_LIGHT3, HIGH);
    client.publish("home/feedback", "Room3 Light On");
    Serial.print("Message: Room3 Light On");
  }
  /*turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message*/
  if (payload[0] == '0')
  {
    digitalWrite(ROOM1_LIGHT3, LOW);
    client.publish("home/feedback", "Room3 Light Off");
  }  
}

}

void reconnect()
{
/* Loop until we're reconnected */
while (!client.connected())
{
Serial.print("Attempting MQTT connection...");
/* Create a random client ID */
String clientId = "Home_automation_UNO";
clientId += String(random(0xffff), HEX);
/* Attempt to connect*/
if (client.connect(clientId.c_str()))
{
Serial.println("connected");
/* Once connected, publish an announcement...*/
/* ... and resubscribe*/
client.subscribe("homedemo/room1/light");
client.subscribe("homedemo/room2/light");
client.subscribe("homedemo/room3/light");

}
else
{
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
/* Wait 5 seconds before retrying*/
delay(5000);
}
}
}

void setup()
{
pinMode(ROOM1_LIGHT1, OUTPUT); // Room1/Light
digitalWrite(ROOM1_LIGHT1, LOW);

pinMode(ROOM1_LIGHT2, OUTPUT); //Room2/Light
digitalWrite(ROOM1_LIGHT2, LOW);

pinMode(ROOM1_LIGHT3, OUTPUT); //Room3/Light
digitalWrite(ROOM1_LIGHT3, LOW);

pinMode(ROOM1_Temperature, INPUT); // Room1/Temparature
pinMode(ROOM2_Temperature, INPUT); // Room2/Temparature
pinMode(ROOM3_Temperature, INPUT); // Room3/Temparature

Serial.begin(9600);

Ethernet.begin(mac, ip);

if (Ethernet.hardwareStatus() == EthernetNoHardware) {
Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
while (true) {
delay(1); // do nothing, no point running without Ethernet hardware
}
}
if (Ethernet.linkStatus() == LinkOFF) {
Serial.println("Ethernet cable is not connected.");
}
}

void loop()
{
if (!client.connected())
{
/*attempting to connect loop*/
reconnect();
}
if (client.connected())
{
h1 = dht1.readHumidity();
t1 = dht1.readTemperature();
hic1 = dht1.computeHeatIndex(t1, h1, false);

h2 = dht2.readHumidity();
t2 = dht2.readTemperature();
hic2 = dht2.computeHeatIndex(t2, h2, false);

h3 = dht3.readHumidity();
t3 = dht3.readTemperature();
hic3 = dht3.computeHeatIndex(t3, h3, false);
dtostrf(hic1,4,4,temp1);
dtostrf(hic2,4,4,temp2);
dtostrf(hic3,4,4,temp3);
client.publish("home/room1/temparature", temp1);
Serial.print(temp1);
client.publish("home/room2/temparature",temp2);
Serial.print(temp2);
client.publish("home/room3/temparature",temp3);
Serial.print(temp3);
delay(1000);
}
client.loop();
}