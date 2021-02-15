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

const char* ssid = "Netcore_2_4G";//修改，你的路由去WIFI名字
const char* password = "qydcos123$";//你的WIFI密码
const char* mqtt_server = "bemfa.com";//默认，MQTT服务器
const int mqtt_server_port = 9501;//默认，MQTT服务器
#define ID_MQTT  "1c0b7b3809c7b1ad9bb32cbbaf8fbcbc"     //修改，你的Client ID
const char*  topic = "mlserver";  //主题名字，可在巴法云控制台自行创建，名称随意


WiFiClient espClient;
PubSubClient client(espClient);

// 建立WiFiManager对象
WiFiManager wifiManager;

//灯光函数及引脚定义
void turnOnLed();
void turnOffLed();
const int B_led = 02;


void setup_wifi() {
  delay(10);
  // 清除ESP8266所存储的WiFi连接信息以便测试WiFiManager工作效果
  // wifiManager.resetSettings();
  //Serial.println("ESP8266 WiFi Settings Cleared");
  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
  
  wifiManager.autoConnect("AutoConnectAP");

  // 如果您希望该WiFi添加密码，可以使用以下语句：
  // wifiManager.autoConnect("AutoConnectAP", "12345678");
  // 以上语句中的12345678是连接AutoConnectAP的密码

  // WiFi连接成功后将通过串口监视器输出连接成功信息

  Serial.print("ESP8266 Connected to ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:" + WiFi.localIP());
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String Mqtt_Buff = "";
  for (int i = 0; i < length; i++) {
    Mqtt_Buff += (char)payload[i];
  }
  Serial.print(Mqtt_Buff);
  // Switch on the LED if an 1 was received as first character
  if (Mqtt_Buff == "on") {//如果接收字符on，亮灯
    turnOnLed();//开灯函数

  } else if (Mqtt_Buff == "off") {//如果接收字符off，亮灯
    turnOffLed();//关灯函数
  }
  Mqtt_Buff = "";
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    if (client.connect(ID_MQTT)) {
      client.subscribe(topic);//修改，修改为你的主题
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(B_led, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_server_port);
  client.setCallback(callback);

}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

//打开灯泡
void turnOnLed() {
  Serial.println("turn on light");
  digitalWrite(B_led, HIGH);
}
//关闭灯泡
void turnOffLed() {
  Serial.println("turn off light");
  digitalWrite(B_led, LOW);
}