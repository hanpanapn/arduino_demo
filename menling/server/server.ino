/*
  Time:20200508
  Author: youngda
  QQ:1217882800
  wechat:19092550573
  QQ群：824273231
  项目示例：通过发送on或off控制开关
*/

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "PubSubClient.h"//默认，加载MQTT库文件

const char* mqtt_server = "bemfa.com";//默认，MQTT服务器
const int mqtt_server_port = 9501;//默认，MQTT服务器
#define ID_MQTT  "1c0b7b3809c7b1ad9bb32cbbaf8fbcbc"     //修改，你的Client ID
const char*  topic = "mlserver";  //主题名字，可在巴法云控制台自行创建，名称随意


WiFiClient espClient;
PubSubClient client(espClient);
// 建立WiFiManager对象
WiFiManager wifiManager;
const int pushButton = D4;
void setup_wifi() {
  delay(2000);
  // 清除ESP8266所存储的WiFi连接信息以便测试WiFiManager工作效果
  // wifiManager.resetSettings();
  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
  wifiManager.autoConnect("AutoConnectAP");
  // 如果您希望该WiFi添加密码，可以使用以下语句：
  // wifiManager.autoConnect("AutoConnectAP", "12345678");
  Serial.print("ESP8266 Connected to ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:" + WiFi.localIP());
}

void setup() {
  pinMode(pushButton, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_server_port);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("ddf");
   client.connect(ID_MQTT)
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  } else {
    int buttonState = digitalRead(pushButton);
    if (buttonState == 0) {
      Serial.println("button push start");
       
      const char* onstr = "on";
      const char* offstr = "off";
      bool cc = client.publish(topic, onstr); //修改，修改为你的主题
      Serial.println(cc);
      Serial.println("button push end");
      delay(500);
      client.publish(topic, offstr); //修改，修改为你的主题
    }
  }

  client.loop();
}

