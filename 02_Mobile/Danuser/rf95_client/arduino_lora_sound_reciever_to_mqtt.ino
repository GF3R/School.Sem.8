#include <SPI.h>
#include <RH_RF95.h>
#include <YunClient.h>
#include <PubSubClient.h>

#define MQTT_SERVER "siot.net"
#define MQTT_CLIENTID "YUN-Sensor"
#define PUB_TOPIC "test/publish/noise"
#define SUB_TOPIC "test/subscribe/noise"

RH_RF95 rf95;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(topic);
  Serial.print((char*)payload);
}

YunClient yun;
PubSubClient mqtt(MQTT_SERVER, 1883, callback, yun);




void setup() 
{
  Serial.begin(9600);
    Bridge.begin();
    

  while (!Serial) ; 
  if (!rf95.init())
    Serial.println("init failed");
  
  if (mqtt.connect(MQTT_CLIENTID)) {
    mqtt.subscribe(SUB_TOPIC);
    Serial.println("mqtt subscribed");
  }
  rf95.setTxPower(23, false);
  rf95.setFrequency(868.0);
  
}

void loop()
{
  mqtt.loop();
  /**
  Serial.println("Sending to rf95_server");
  uint8_t data[] = "Hello World!";
  rf95.send(data, sizeof(data));
  **/
  
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  if (rf95.waitAvailableTimeout(3000))
  {  
    if (rf95.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf); 
      mqtt.publish(PUB_TOPIC,(char*)buf);
    }
    else{
      Serial.println("nothing recieved");
    }
  }
  else{
    Serial.println("Server down");
  }
  delay(400);
}


