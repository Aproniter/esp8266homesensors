// #include <Arduino.h>
// #include <SPI.h>
// #include "FS.h"
// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <WiFiClient.h>
// #include <ESPAsyncWebServer.h>
// #include <FastBot.h>
// #include <ESP8266mDNS.h>
// #include <Wire.h>
// #include <Adafruit_BMP085.h>
// #include <AsyncElegantOTA.h>
// #include <Arduino_JSON.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #include <WiFiServerSecureBearSSL.h>
// #include <ESP8266HTTPClient.h>
// #include "config.h"


// const int LedPin = D3;
// const int LigntSensor = D5;

// WiFiClient wifiClient;
// AsyncWebServer server(8080);
// AsyncEventSource events("/events");
// FastBot bot(BOT_TOKEN);
// unsigned long wifiMillis = 0;
// unsigned long wifiInterval = 30000;
// boolean wifi_reconnect = true;

// const int utcOffsetInSeconds = 10800;
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// unsigned long sensorMillis = 0;
// unsigned long lightMillis = 0;
// unsigned long sensorInterval = 60000;
// unsigned long lightInterval = 5000;
// Adafruit_BMP085 bmp;
// int old_light_level;
// int light_level = 0;
// int led_level = 0;
// float temperature = 0.0;
// float pressure = 0.0;
// int backlight = 0;
// bool backlight_flag = false;
// bool autoled = true;
// int counter = 0;


// void postSensorsData(){
//   // String host = "functions.yandexcloud.net/d4e366ggusnraqeprlia";
//   String host = "https://jigsaw.w3.org/HTTP/connection.html";
//   std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
//   client->setInsecure();
//   HTTPClient https;

//   if (https.begin(*client, host)) {  // HTTPS
//     Serial.println("[HTTPS] GET...");
//     int httpCode = https.GET();

//     // httpCode will be negative on error
//     if (httpCode > 0) {
//       // HTTP header has been send and Server response header has been handled
//       Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
//       // file found at server?
//       // if (httpCode == HTTP_CODE_OK) {
//       //   String payload = https.getString();
//       //   Serial.println(String("[HTTPS] Received payload: ") + payload);
//       //   Serial.println(String("1BTC = ") + payload + "USD");
//       // }
//     } else {
//       Serial.printf("[HTTPS] GET... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
//     }

//     https.end();
//   } else {
//     Serial.printf("[HTTPS] Unable to connect\n\r");
//   }


//   // while (https.connected() ) {
//   //   size_t size = stream->available();
//   //   if (size) {
//   //     int c = stream->readBytes(char_buff, ((size > sizeof(char_buff)) ? sizeof(char_buff) : size));
//   //     if (c > 0){
//   //       Serial.println(c);
//   //     }
//   //   }
//   // }


  
//   // HTTPClient https;
//   // // https.begin(*client, "https://functions.yandexcloud.net/d4e366ggusnraqeprlia");
//   // https.begin(*client, "https://jigsaw.w3.org/HTTP/connection.html");
//   // char payload[] = "{}";
//   // // https.addHeader("Accept", "*/*");
//   // // https.addHeader("Content-Length", String(strlen(payload)));
//   // // https.addHeader("Content-Type", "application/json");
//   // // https.addHeader("User-Agent", "curl/7.58.0");
  
//   // int httpCode = https.GET();
//   // Serial.println(httpCode);
//   // Serial.println(https.getString());
//   // https.end();
// }

//   // String host = "functions.yandexcloud.net/d4e366ggusnraqeprlia";
//   // WiFiClientSecure *client = new WiFiClientSecure;
//   // client->setInsecure();
//   // HTTPClient https;
//   // https.begin(*client, "https://functions.yandexcloud.net/d4e366ggusnraqeprlia");
//   // https.addHeader("Content-Type", "application/json");
//   // https.addHeader("Host", "functions.yandexcloud.net");
//   // String payload = "{\"data\":\"data\"}";
//   // int httpCode = https.GET();
//   // Serial.println(httpCode);
//   // Serial.println(https.getString());
//   // https.end();

// void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//   JSONVar myObject = JSON.parse((char*)data);
//   if (JSON.typeof(myObject) == "undefined") {
//     Serial.println("Parsing input failed!");
//   } else {
//     JSONVar value = myObject["ledlevel"];
//     led_level = int(value);
//     File f = SPIFFS.open("/led_level.txt", "w");
//     if (!f) {
//         Serial.println("file open failed");
//     } else {
//       f.print(led_level);
//       f.close();
//       Serial.println(led_level);
//     }
//   }
// }

// int getLightLevel(){
//   digitalWrite(LigntSensor, HIGH);
//   int level = analogRead(A0);
//   digitalWrite(LigntSensor, LOW);
//   return level;
// }

// void getSensorsData(){
//   temperature = round(int(bmp.readTemperature() * 100));
//   pressure = round(int(bmp.readPressure() * 0.00750063755419211 * 100));
// }

// void ledOn(){
//   int oldLightLevel = light_level;
//   Serial.println(oldLightLevel);
//   digitalWrite(LedPin, HIGH);
//   light_level = getLightLevel();
//   backlight = light_level - oldLightLevel;
//   Serial.println(light_level);
//   Serial.println(backlight);
// }

// void ledOff(){
//   digitalWrite(LedPin, LOW);
//   backlight = 0;
// }

// String getJSONSensorsData(){
//     JSONVar myObject;
//     myObject["temperature"] = temperature;
//     myObject["pressure"] = pressure;
//     myObject["lightlevel"] = light_level;
//     String jsonString = JSON.stringify(myObject);
//     return jsonString;
// }

// String getDateTime(){
//     time_t epochTime = timeClient.getEpochTime();
//     struct tm *ptm = gmtime((time_t *)&epochTime);
//     String dateTime = String(ptm->tm_mday) + "-" + String(ptm->tm_mon+1) + "-" + String(ptm->tm_year+1900);
//     dateTime += ".";
//     dateTime += timeClient.getFormattedTime();
//     return dateTime;
// }

// void initServer() {
//   server.on("/ledOn", HTTP_GET, [](AsyncWebServerRequest *request) {
//       if(!request->authenticate(http_username, http_password)){
//         return request->requestAuthentication();
//       }
//       // digitalWrite(LedPin, HIGH);
//       request->send(200, "application/json", "{\"led\": \"ON\"}");
//   });
//   server.on("/ledOff", HTTP_GET, [](AsyncWebServerRequest *request) {
//       if(!request->authenticate(http_username, http_password)){
//         return request->requestAuthentication();
//       }
//       // digitalWrite(LedPin, LOW);
//       request->send(200, "application/json", "{\"led\": \"OFF\"}");
//   });
//   server.on("/autoledOn", HTTP_GET, [](AsyncWebServerRequest *request) {
//       if(!request->authenticate(http_username, http_password)){
//         return request->requestAuthentication();
//       }
//       autoled = true;
//       request->send(200, "application/json", "{\"auto\": \"ON\"}");
//   });
//   server.on("/autoledOff", HTTP_GET, [](AsyncWebServerRequest *request) {
//       if(!request->authenticate(http_username, http_password)){
//         return request->requestAuthentication();
//       }
//       autoled = false;
//       request->send(200, "application/json", "{\"auto\": \"OFF\"}");
//   });
//   server.on("/lightStatus", HTTP_GET, [](AsyncWebServerRequest *request){
//     JSONVar myObject;
//     myObject["led"] = digitalRead(LedPin)?"ON":"OFF";
//     String jsonString = JSON.stringify(myObject);
//     AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonString);
//     response->addHeader("Access-Control-Allow-Origin", "*");
//     request->send(response);
//   });
//   server.on("/settingsLight", HTTP_POST, [](AsyncWebServerRequest *request){
//     if(!request->authenticate(http_username, http_password)){
//         return request->requestAuthentication();
//     }
//     AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"OK\":\"OK\"}");
//     response->addHeader("Access-Control-Allow-Origin", "*");
//     request->send(response);
//   }, NULL, handleBody);
//   server.on("/sensorsStatus", HTTP_GET, [](AsyncWebServerRequest *request){
//     String jsonString = getJSONSensorsData();
//     AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonString);
//     response->addHeader("Access-Control-Allow-Origin", "*");
//     request->send(response);
//   });
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//       request->send(SPIFFS, "/index.html", "text/html");
//   });
//   AsyncElegantOTA.begin(&server, http_username, http_password);
//   server.addHandler(&events);
//   server.begin();
//   Serial.println("HTTP server started");
// }

// void setup() {
//   pinMode(LigntSensor, OUTPUT);
//   pinMode(LedPin, OUTPUT);
//   digitalWrite(LigntSensor, LOW);
//   digitalWrite(LedPin, LOW);
//   bmp.begin();
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(WIFI_SSID, WIFI_PASS);
//   initServer();
//   getSensorsData();
//   SPIFFS.begin();
//   File f = SPIFFS.open("/led_level.txt", "r");
//     if (!f) {
//         Serial.println("file open failed");
//     } else {
//       String datafile;
//       while (f.available()) {
//         datafile = f.readStringUntil('\n');
//       }
//       led_level = datafile.toInt();
//       f.close();
//     }
//   timeClient.begin();
// }

// void loop() {
//   unsigned long currentMillis = millis();
//   if(currentMillis - lightMillis >= lightInterval){
//     light_level = getLightLevel();
//     lightMillis = currentMillis;
//     if(autoled){
//       Serial.println(old_light_level);
//       Serial.println(light_level);
//       Serial.println(backlight);
//       if((light_level - backlight) <= led_level){
//         if(!digitalRead(LedPin)){
//           old_light_level = light_level;
//           backlight_flag = true;
//           digitalWrite(LedPin, HIGH);
//         }

//       } else {
//         if(backlight_flag){
//           backlight = light_level - old_light_level;
//           backlight_flag = false;
//         } else {
//           digitalWrite(LedPin, LOW);
//           backlight = 0;
//         }
//       }
//     }
//     String jsonString = getJSONSensorsData();
//     events.send(jsonString.c_str(), NULL, millis());
//   }
//   if(currentMillis - sensorMillis >= sensorInterval){
//     getSensorsData();
//     sensorMillis = currentMillis;
//     // File sensors = SPIFFS.open("/sensors.json", "a");
//     // if (!sensors) {
//     //     Serial.println("file open failed");
//     // } else {
      
//     //   JSONVar myObject;
//     //   myObject[getDateTime()] = getJSONSensorsData();
//     //   String jsonString = JSON.stringify(myObject);
//     //   sensors.print(jsonString);
//     //   sensors.println("|||");
//     //   sensors.close();
//     // }
//   }
//   if ((WiFi.status() != WL_CONNECTED) && (currentMillis - wifiMillis >= wifiInterval)) {
//     WiFi.disconnect();
//     WiFi.begin(WIFI_SSID, WIFI_PASS);
//     wifiMillis = currentMillis;
//     wifi_reconnect = true;
//   } else {
//     if ((WiFi.status() == WL_CONNECTED) && wifi_reconnect){
//       String IP = WiFi.localIP().toString();
//       IP.concat(":8080");
//       // bot.sendMessage(IP, String(CHAT_ID)); 
//       Serial.println(IP);
//       wifi_reconnect = false;
//       timeClient.update();
//       postSensorsData();
//       // File f = SPIFFS.open("/sensors.json", "r");
//       // if (!f) {
//       //     Serial.println("file open failed");
//       // } else {
//       //   bot.sendFile(f, FB_DOC, (getDateTime() + ".json"), String(CHAT_ID));
//       //   f.close();
//       //   SPIFFS.remove("/sensors.json");
//       // }
//     }
//   }
// }

#include <Arduino.h>
#include <SPI.h>
#include "FS.h"
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"

int LedPin = D3;
int LigntSensor = D5;

unsigned long wifi_millis = 0;
const int wifi_interval = 30000;
bool wifi_reconnect = true;

// const int utc_offset_in_seconds = 10800;
const int utc_offset_in_seconds = 0;
WiFiUDP ntp_udp;
NTPClient time_client(ntp_udp, "pool.ntp.org", utc_offset_in_seconds);

Adafruit_BMP085 bmp;
unsigned long temp_press_millis = 0;
unsigned long light_level_millis = 0;
unsigned long post_millis = 0;
unsigned long temp_press_interval = 300000;
unsigned long light_level_interval = 5000;
unsigned long post_interval = 3600000;
// int led_level = 0;
// int old_light_level;
int light_level = 0;
int temperature = 0;
int pressure = 0;

void getTempAndPress(){
  temperature = round(int(bmp.readTemperature() * 100));
  pressure = round(int(bmp.readPressure() * 0.00750063755419211 * 100));
}

void setup() {
  pinMode(LigntSensor, OUTPUT);
  pinMode(LedPin, OUTPUT);
  digitalWrite(LigntSensor, HIGH);
  digitalWrite(LedPin, LOW);
  bmp.begin();
  SPIFFS.begin();
  Serial.begin(115200);
  time_client.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  light_level = analogRead(A0);
  getTempAndPress();
  SPIFFS.remove("/sensors.json");
}

// String getDateTime(String& date_time){
//     // .reserve(20);
//     time_t epoch_time = time_client.getEpochTime();
//     struct tm *ptm = gmtime((time_t *)&epoch_time);
//     date_time += ptm->tm_mday;
//     date_time += F("-");
//     date_time += ptm->tm_mon+1;
//     date_time += F("-");
//     date_time += ptm->tm_year+1900;
//     date_time += F(".");
//     date_time += time_client.getFormattedTime();
//     return date_time;
// }

void postSensorsDataToYandexObjectStorage() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  if (https.begin(*client, "https://functions.yandexcloud.net/d4e366ggusnraqeprlia")) {
    Serial.println("[HTTPS] POST...");
    File f = SPIFFS.open("/sensors.json", "r");
    if (!f) {
        Serial.println("file open failed");
    } else {
      Serial.println(f.size());
      String datafile;
      while (f.available()) {
        datafile += f.readString();
      }
      datafile += time_client.getEpochTime();
      int httpCode = https.POST(datafile);
      if (httpCode > 0) {
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
        String payload = https.getString();
        Serial.println(payload);
        f.close();
        SPIFFS.remove("/sensors.json");
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
    
  } else {
    Serial.printf("[HTTPS] Unable to connect\n\r");
  }
}

String string_to_file(String& temp_press_data){
  temp_press_data += F("{\"t\":\"");
  temp_press_data += temperature;
  temp_press_data += F("\",\"p\":\"");
  temp_press_data += pressure;
  temp_press_data += F("\",\"l\":\"");
  temp_press_data += light_level;
  temp_press_data += F("\"}");
  return temp_press_data;
}

void loop() {
  unsigned long current_millis = millis();
  if(WiFi.status() == WL_CONNECTED){
    if(wifi_reconnect){
      time_client.update();

    }
      wifi_reconnect = false;
  }
  if ((WiFi.status() != WL_CONNECTED) && (current_millis - wifi_millis >= wifi_interval)) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    wifi_millis = current_millis;
    wifi_reconnect = true;
  }
  if(current_millis - light_level_millis >= light_level_interval){
    light_level = analogRead(A0);
    light_level_millis = current_millis;
  }
  if(current_millis - temp_press_millis >= temp_press_interval){
    getTempAndPress();
    temp_press_millis = current_millis;
    String to_file;
    string_to_file(to_file);
    // Serial.println(to_file);
    // Serial.println(current_millis/1000);
    // Serial.println(time_client.getEpochTime());
    // Serial.println(time_client.getEpochTime()%10);
    File sensors = SPIFFS.open("/sensors.json", "a");
    if (!sensors) {
        Serial.println("file open failed");
    } else {
      sensors.print(to_file);
      sensors.println(F("|||"));
      sensors.close();
    }
  }
  if((time_client.getEpochTime() > 1000000000) and (WiFi.status() == WL_CONNECTED)){
    if(current_millis - post_millis >= post_interval){
      time_client.update();
      postSensorsDataToYandexObjectStorage();
      post_millis = current_millis;
    }
  }
}