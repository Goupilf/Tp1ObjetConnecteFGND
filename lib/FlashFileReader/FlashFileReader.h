#pragma once
#include <Arduino.h>
#include <FS.h>
#include <Spiffs.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

class FlashFileReader
{
public:
    FlashFileReader();
    bool loadFromSpiffs(String path,WebServer* server);
    
private:
    
};