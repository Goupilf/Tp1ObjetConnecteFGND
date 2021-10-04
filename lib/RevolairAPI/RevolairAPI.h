#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include <Spiffs.h>
#include <FS.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

class RevolairAPI
{
public:
    RevolairAPI(const char* API_URL, const char* API_TOKEN);
    void postJSON(String& encodedJSON);
    void sendPM25ValueToAPI(int pm2p5value);
    
private:
    const char* API_URL;
    const char* API_TOKEN;
};