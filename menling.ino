#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#define BLINKER_MQTT
#include <Blinker.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
const char* fingerprint = "f1a76dcc044882a3783d3eaf61c90ef5f177bffb";

char* ssid;
char* pswd;

char* auth = "b1d1699253a0";

//门铃数据针脚
const int  data_ping = 2; //D4

// 建立WiFiManager对象
WiFiManager wifiManager;
// 新建组件对象
BlinkerButton Button1("btn-d2k");
BlinkerButton Button2("btn-x45");


void getPostData() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  client->setFingerprint(fingerprint);

  HTTPClient https;

  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, "https://www.fastmock.site/mock/5a6d13e1c4fa471aef84f58c8df37ec1/mengling/send")) {  // HTTPS
    Serial.print("[HTTPS] GET...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();

    String payload = https.getString();
    Serial.println(payload);

    https.end();
  }

}
// 按下按键即会执行该函数
void button1_callback(const String & state) {
  pushMenling();
  getPostData();

}
void pushMenling() {
  digitalWrite(data_ping, HIGH);
  delay(200);
  digitalWrite(data_ping, LOW);
  delay(200);

  digitalWrite(data_ping, HIGH);
  delay(200);
  digitalWrite(data_ping, LOW);
  delay(600);

  digitalWrite(data_ping, HIGH);
  delay(200);
  digitalWrite(data_ping, LOW);
  delay(200);

  digitalWrite(data_ping, HIGH);
  delay(200);
  digitalWrite(data_ping, LOW);
}

void button2_callback(const String & state) {
  wifiManager.resetSettings();
  BLINKER_LOG("will be restart");
  system_restart();
}

void setup() {
  delay(500);
  Serial.begin(115200);

  pinMode(data_ping, OUTPUT);
  BLINKER_DEBUG.stream(Serial);



  // 清除ESP8266所存储的WiFi连接信息以便测试WiFiManager工作效果
  // wifiManager.resetSettings();
  Serial.println("ESP8266 WiFi Settings Cleared");
  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
  wifiManager.autoConnect("AutoConnectAP");

  // 如果您希望该WiFi添加密码，可以使用以下语句：
  // wifiManager.autoConnect("AutoConnectAP", "12345678");
  // 以上语句中的12345678是连接AutoConnectAP的密码

  // WiFi连接成功后将通过串口监视器输出连接成功信息

  Serial.print("ESP8266 Connected to ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // IP
  String s = WiFi.SSID();
  String p = WiFi.psk();
  ssid = &s[0];
  pswd =  &p[0];
  delay(300);
  Blinker.begin(auth, ssid, pswd);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);

}

void loop() {
  Blinker.run();
}
