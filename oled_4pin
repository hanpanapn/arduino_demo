#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,SCL,SDA,U8X8_PIN_NONE);

void setup(void) 
{
  u8g2.begin();
}

void loop(void) 
{
  u8g2.clearBuffer();  //清除缓存         
  u8g2.setFont(u8g2_font_ncenB08_tr); 
  u8g2.drawStr(0,10,"Hello World!");  
  u8g2.drawStr(0,20,"This is bro cainiao!");  
  u8g2.drawStr(0,30,"Welcome to U8G2!");  
  u8g2.sendBuffer();  //显示      
  delay(1000);  
}
