#include <M5Atom.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include "config.h"

WiFiUDP udp;
const char *ssid     = SSID;
const char *password = PASSWORD;
const IPAddress ip (192,168,1,74);
const IPAddress gateway(192, 168, 1, 70);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress dns1(192, 168, 1, 78);
const IPAddress castaddress(255,255,255,255);
const String ccmname ="test";
const String ccmtype ="cMC";
const String Sensorname= "FLOW";
const String room = "1";
const String region = "12";
const String order = "1";
const String priority = "1";
const String host = "192.168.1.71";
const int port = 16520;

const int sensor_pin = 26;
WiFiMulti WiFiMulti;

volatile int pulseCount;
float ryuuryou;
unsigned long oldTime;
float sam_ryuuryou=0;
float keisuu=5.5; //流量調整係数
void setup(){
pinMode(sensor_pin, INPUT_PULLUP);
oldTime = millis();

    M5.begin(true, false, true);
    pinMode(0,OUTPUT);
    digitalWrite(0,LOW);
    Serial.begin(115200);
    WiFiMulti.addAP(
      ssid,
      password);
    if (!WiFi.config(ip,gateway,subnet,dns1)){
      Serial.println("Failed to configure!");
    }
    M5.dis.fillpix(0xff0000);
    while (WiFiMulti.run() !=
     WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    M5.dis.fillpix(0x00ff00);
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
void loop(){
attachInterrupt(digitalPinToInterrupt(sensor_pin), flow, RISING); 
if(millis() >= (oldTime + 1000)){
oldTime=millis();
ryuuryou=((pulseCount / keisuu ));
//sam_ryuuryou=sam_ryuuryou + ryuuryou;
pulseCount = 0;

    CCM_send(Sensorname,ryuuryou); 
    //Serial.println(sam_ryuuryou);
    //delay(1000);
}
}
void flow (){
pulseCount++;
}

int CCM_send (String s_name,float val){
   String msg;
   msg  = "<?xml version=\"1.0\" ?>\r\n<UECS ver=\"1.00-E10\" >\r\n";
   msg += "<DATA type=\"" + ccmname + s_name + "." + ccmtype + "\" ";
   msg += "room=\"" + room + "\" ";
   msg += "region=\"" + region + "\" ";
   msg += "order=\"" + order + "\" ";
   msg += "priority=\"" + priority + "\" >\r\n";
 //  msg += val;
   msg += String(val,2);
   msg += "\r\n</DATA>\r\n";
   msg += "<IP>"+host+"</IP></UECS>";
   Serial.println(msg);
   udp.beginPacket(castaddress,port);
   udp.println(msg);
   udp.endPacket();
   delay(100);
  int n = Serial.println(msg);
return n;
}
