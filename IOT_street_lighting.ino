#define TCP_MSS 0
#define LWIP_IPV6 0
#define LWIP_FEATURES 0 
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include"pin_config.h"

const char* ssid = "esa"; // Enter your WiFi name
const char* password =  "1234567711"; // Enter WiFi password
const char* mqttServer = "learning.masterofthings.com";
const int mqttPort = 1883;
const char* mqttUser = "MoTSensorKitv2.0";
const char* mqttPassword = "MoTSensorKitv2.0Pass";
WiFiClient espClient;
PubSubClient client(espClient);

long int millis_now;
int i=0,intensity=0,sensor_intensity, sensor_ir, sensor_elec, lamp_state;
char topic_end=0;char byteRead;
char *str="";

void callback(char* topic, byte* payload, unsigned int length) {
   intensity=0;
  for (int i = 0; i < length; i++) {   
  intensity*=10;    
  intensity += ((char)payload[i]-'0');  
   }
char l= strlen(topic);
topic_end=topic[l-1];
}


void setup() {
  pinMode(D0, INPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }  
  Serial.println("Connected to the WiFi network");
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000); 
    }
  }
  client.subscribe("Phone/Minia/#");
  client.subscribe("Phone/Minia");
}
 
void loop() {
client.loop();    
switch(topic_end)
{
  case '1':
  analogWrite(lamp1,(intensity*10));
  break;  
  case '2':
  analogWrite(lamp2,(intensity*10));
  break;  
  case '3':
  analogWrite(lamp3,(intensity*10));
  break;  
  case '6':
  analogWrite(lamp11,(intensity*10));
  break;  
  case '7':
  analogWrite(lamp12,(intensity*10));
  break;
  case '8':
  analogWrite(lamp13,(intensity*10));
  break;  

  case 'r':
  analogWrite(lamp1,(intensity*10));
  analogWrite(lamp2,(intensity*10));
  analogWrite(lamp3,(intensity*10));
  analogWrite(lamp4,(intensity*10));
  break;  
  case 'l':
  analogWrite(lamp11,(intensity*10));
  analogWrite(lamp12,(intensity*10));
  analogWrite(lamp13,(intensity*10));
  break;  
  case 'e':
  analogWrite(lamp2,(intensity*10));
  analogWrite(lamp4,(intensity*10));
  analogWrite(lamp12,(intensity*10));
    break;  
  case 'o':
  analogWrite(lamp1,(intensity*10));
  analogWrite(lamp3,(intensity*10));
  analogWrite(lamp11,(intensity*10));
  analogWrite(lamp13,(intensity*10));
  break;  
  case 'y':
  analogWrite(lamp1,(intensity*10));
  analogWrite(lamp2,(intensity*10));
  analogWrite(lamp3,(intensity*10));
  analogWrite(lamp4,(intensity*10));
  analogWrite(lamp11,(intensity*10));
  analogWrite(lamp12,(intensity*10));
  analogWrite(lamp13,(intensity*10));
    break;  
  case 'a':
  analogWrite(lamp1,(intensity*10));
  analogWrite(lamp2,(intensity*10));
  analogWrite(lamp3,(intensity*10));
  analogWrite(lamp4,(intensity*10));
  analogWrite(lamp11,(intensity*10));
  analogWrite(lamp12,(intensity*10));
  analogWrite(lamp13,(intensity*10));
  break;   
  case 't':
  analogWrite(lamp1,(intensity*10));
  analogWrite(lamp2,(intensity*10));
  analogWrite(lamp3,(intensity*10));
  analogWrite(lamp4,(intensity*10));
  analogWrite(lamp11,(intensity*10));
  analogWrite(lamp12,(intensity*10));
  analogWrite(lamp13,(intensity*10));
  break;   
  }
millis_now=millis();
if(millis_now%5000==0){
sensor_intensity=analogRead(A0);
sensor_ir=digitalRead(D0)*5;
sensor_elec=digitalRead(D2); 
sprintf(str, "%d",sensor_intensity );
client.publish("Phone/ESPkit/light",str); //Topic name
sprintf(str, "%d", sensor_ir);
client.publish("Phone/ESPkit/ir",str); //Topic name

if(sensor_ir==5){
  client.publish("Phone/ESPkit/state","stolen");  
}
else if(sensor_ir==0)
{
  if(sensor_elec==0){
  client.publish("Phone/ESPkit/state","Off"); 
  }
  else if(sensor_elec==1)
  {
    if(sensor_intensity>=500)
    client.publish("Phone/ESPkit/state","on");
    if(sensor_intensity<500)
    client.publish("Phone/ESPkit/state","broken");
  }
}
Serial.println(".");
}
}
