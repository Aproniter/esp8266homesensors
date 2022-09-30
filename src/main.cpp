#include <Arduino.h>
#include <SPI.h>
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <FastBot.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <AsyncElegantOTA.h>
#include <Arduino_JSON.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"

const int LedPin = D3;
const int LigntSensor = D5;

WiFiClient wifiClient;
AsyncWebServer server(8080);
AsyncEventSource events("/events");
FastBot bot(BOT_TOKEN);
unsigned long wifiMillis = 0;
unsigned long wifiInterval = 30000;
boolean wifi_reconnect = true;

const int utcOffsetInSeconds = 10800;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

unsigned long sensorMillis = 0;
unsigned long lightMillis = 0;
unsigned long sensorInterval = 60000;
unsigned long lightInterval = 5000;
Adafruit_BMP085 bmp;
int old_light_level;
int light_level = 0;
int led_level = 0;
float temperature = 0.0;
float pressure = 0.0;
int backlight = 0;
bool backlight_flag = false;
bool autoled = true;
int counter = 0;


void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  JSONVar myObject = JSON.parse((char*)data);
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
  } else {
    JSONVar value = myObject["ledlevel"];
    led_level = int(value);
    File f = SPIFFS.open("/led_level.txt", "w");
    if (!f) {
        Serial.println("file open failed");
    } else {
      f.print(led_level);
      f.close();
      Serial.println(led_level);
    }
  }
}

int getLightLevel(){
  digitalWrite(LigntSensor, HIGH);
  int level = analogRead(A0);
  digitalWrite(LigntSensor, LOW);
  return level;
}

void getSensorsData(){
  temperature = round(int(bmp.readTemperature() * 100));
  pressure = round(int(bmp.readPressure() * 0.00750063755419211 * 100));
}

void ledOn(){
  int oldLightLevel = light_level;
  Serial.println(oldLightLevel);
  digitalWrite(LedPin, HIGH);
  light_level = getLightLevel();
  backlight = light_level - oldLightLevel;
  Serial.println(light_level);
  Serial.println(backlight);
}

void ledOff(){
  digitalWrite(LedPin, LOW);
  backlight = 0;
}

String getJSONSensorsData(){
    JSONVar myObject;
    myObject["temperature"] = temperature;
    myObject["pressure"] = pressure;
    myObject["lightlevel"] = light_level;
    String jsonString = JSON.stringify(myObject);
    return jsonString;
}

String getDateTime(){
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);
    String dateTime = String(ptm->tm_mday) + "-" + String(ptm->tm_mon+1) + "-" + String(ptm->tm_year+1900);
    dateTime += ".";
    dateTime += timeClient.getFormattedTime();
    return dateTime;
}

void initServer() {
  server.on("/ledOn", HTTP_GET, [](AsyncWebServerRequest *request) {
      if(!request->authenticate(http_username, http_password)){
        return request->requestAuthentication();
      }
      digitalWrite(LedPin, HIGH);
      request->send(200, "application/json", "{\"led\": \"ON\"}");
  });
  server.on("/ledOff", HTTP_GET, [](AsyncWebServerRequest *request) {
      if(!request->authenticate(http_username, http_password)){
        return request->requestAuthentication();
      }
      digitalWrite(LedPin, LOW);
      request->send(200, "application/json", "{\"led\": \"OFF\"}");
  });
  server.on("/autoledOn", HTTP_GET, [](AsyncWebServerRequest *request) {
      if(!request->authenticate(http_username, http_password)){
        return request->requestAuthentication();
      }
      autoled = true;
      request->send(200, "application/json", "{\"auto\": \"ON\"}");
  });
  server.on("/autoledOff", HTTP_GET, [](AsyncWebServerRequest *request) {
      if(!request->authenticate(http_username, http_password)){
        return request->requestAuthentication();
      }
      autoled = false;
      request->send(200, "application/json", "{\"auto\": \"OFF\"}");
  });
  server.on("/lightStatus", HTTP_GET, [](AsyncWebServerRequest *request){
    JSONVar myObject;
    myObject["led"] = digitalRead(LedPin)?"ON":"OFF";
    String jsonString = JSON.stringify(myObject);
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonString);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/settingsLight", HTTP_POST, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password)){
        return request->requestAuthentication();
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"OK\":\"OK\"}");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  }, NULL, handleBody);
  server.on("/sensorsStatus", HTTP_GET, [](AsyncWebServerRequest *request){
    String jsonString = getJSONSensorsData();
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonString);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html");
  });
  AsyncElegantOTA.begin(&server, http_username, http_password);
  server.addHandler(&events);
  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  pinMode(LigntSensor, OUTPUT);
  pinMode(LedPin, OUTPUT);
  digitalWrite(LigntSensor, LOW);
  digitalWrite(LedPin, LOW);
  bmp.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  initServer();
  getSensorsData();
  SPIFFS.begin();
  File f = SPIFFS.open("/led_level.txt", "r");
    if (!f) {
        Serial.println("file open failed");
    } else {
      String datafile;
      while (f.available()) {
        datafile = f.readStringUntil('\n');
      }
      led_level = datafile.toInt();
      f.close();
    }
  timeClient.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - lightMillis >= lightInterval){
    light_level = getLightLevel();
    lightMillis = currentMillis;
    if(autoled){
      Serial.println(old_light_level);
      Serial.println(light_level);
      Serial.println(backlight);
      if((light_level - backlight) <= led_level){
        if(!digitalRead(LedPin)){
          old_light_level = light_level;
          backlight_flag = true;
          digitalWrite(LedPin, HIGH);
        }

      } else {
        if(backlight_flag){
          backlight = light_level - old_light_level;
          backlight_flag = false;
        } else {
          digitalWrite(LedPin, LOW);
          backlight = 0;
        }
      }
    }
    String jsonString = getJSONSensorsData();
    events.send(jsonString.c_str(), NULL, millis());
  }
  if(currentMillis - sensorMillis >= sensorInterval){
    getSensorsData();
    sensorMillis = currentMillis;
    // File sensors = SPIFFS.open("/sensors.json", "a");
    // if (!sensors) {
    //     Serial.println("file open failed");
    // } else {
      
    //   JSONVar myObject;
    //   myObject[getDateTime()] = getJSONSensorsData();
    //   String jsonString = JSON.stringify(myObject);
    //   sensors.print(jsonString);
    //   sensors.println("|||");
    //   sensors.close();
    // }
  }
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - wifiMillis >= wifiInterval)) {
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    wifiMillis = currentMillis;
    wifi_reconnect = true;
  } else {
    if ((WiFi.status() == WL_CONNECTED) && wifi_reconnect){
      String IP = WiFi.localIP().toString();
      IP.concat(":8080");
      bot.sendMessage(IP, CHAT_ID.toString()); 
      Serial.println(IP);
      wifi_reconnect = false;
      timeClient.update();
      // File f = SPIFFS.open("/sensors.json", "r");
      // if (!f) {
      //     Serial.println("file open failed");
      // } else {
      //   bot.sendFile(f, FB_DOC, (getDateTime() + ".json"), CHAT_ID.toString());
      //   f.close();
      //   SPIFFS.remove("/sensors.json");
      }
    }
  }
}